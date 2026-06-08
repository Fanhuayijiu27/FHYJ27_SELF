# -*- coding: utf-8 -*-
"""
批量导入 COMSOL 边界点并重新仿真（集成 mat2/spf 域检查与自动修正）

功能流程：
1. 每次从干净的源 mph 文件重新打开模型
2. 根据名字对照表修改 widhr1、vocc1、widhr2、vocc2
3. 遍历 comsol_topology_000 ~ comsol_topology_099
4. 读取每个文件夹下 boundary_curve_1.txt ~ boundary_curve_X.txt
5. 自动生成 COMSOL 中的 pol1 ~ polX
6. 构建几何、划分网格
7. 检查材料 mat2 与层流 spf 当前选中域：
   - 正确情况：mat2 和 spf 都选中同一组三个域，并且包含 1 和 3
   - 异常情况：任一模块选中超过三个域，或 mat2/spf 不一致
   - 修正方式：统一保留 1、3、最大域号，写回 mat2 和 spf
8. 运行 sol1
9. 导出温度场 TXT
10. 另存 mph 文件，不污染源 mph

说明：
- 优先用 COMSOL Java API 直接读取 selection().entities()
- 若 Java API 读取失败，则临时保存 mph，并按 check_spf_domains.py 的思路从 dmodel.xml 中解析 entities
"""

from pathlib import Path
import re
import csv
import time
import traceback
import zipfile

import mph


# =========================
# 1. 路径配置
# =========================

MPH_SOURCE = Path(
    r"D:\Diff_Topo\开源\topodiff-Evaluate\3批量导入comsol再仿真\Benchmark\模型结构导入comsolBench.mph"
)

BOUNDARY_ROOT = Path(
    r"D:\Diff_Topo\开源\topodiff-Evaluate\2分块输出边界点"
)

NAME_TABLE = Path(
    r"D:\Diff_Topo\开源\topodiff-Evaluate\1三列数据二值化\名字对照表.txt"
)

EXPORT_ROOT = Path(
    r"D:\Diff_Topo\开源\topodiff-Evaluate\3批量导入comsol再仿真\再仿真温度场"
)

MPH_SAVE_DIR = EXPORT_ROOT / "mph"

LOG_CSV = EXPORT_ROOT / "batch_comsol_log.csv"
DOMAIN_LOG_CSV = EXPORT_ROOT / "domain_selection_log.csv"
DOMAIN_CHECK_TMP_DIR = EXPORT_ROOT / "_tmp_domain_check"


# =========================
# 2. 批量范围配置
# =========================

START_ID = 0
END_ID = 99

# 已有温度场 TXT 时是否跳过
SKIP_IF_EXPORT_EXISTS = False

# 是否保存每个算例对应的 mph
SAVE_MPH = True

# 如果目标 mph 已存在，是否先删除旧文件再保存
DELETE_OLD_MPH = True


# =========================
# 3. 域检查/修正配置
# =========================

# 是否执行“检查并取消多余的域”
CHECK_AND_FIX_DOMAINS = True

# 必须保留的域
REQUIRED_DOMAINS = [1, 3]

# 正常情况下应为三个域：1、3、X
EXPECTED_DOMAIN_COUNT = 3

# 若 mat2 或 spf 不包含 1 和 3，则认为模型选择异常，直接报错停止该算例。
# 如果你想强制修正，可改成 False，但一般不建议。
STRICT_REQUIRED_DOMAINS = True

# Java API 读取失败时，是否临时保存 mph 并解析 dmodel.xml
USE_TEMP_MPH_XML_FALLBACK = True

# 是否保留用于 XML 检查的临时 mph。默认删除，避免占空间。
KEEP_TEMP_DOMAIN_CHECK_MPH = False


# =========================
# 4. 工具函数
# =========================

def ensure_dirs():
    EXPORT_ROOT.mkdir(parents=True, exist_ok=True)
    MPH_SAVE_DIR.mkdir(parents=True, exist_ok=True)
    DOMAIN_CHECK_TMP_DIR.mkdir(parents=True, exist_ok=True)


def normalize_case_id(raw_id):
    """
    将 0、000、comsol_topology_000 等统一成 000
    """
    raw_id = str(raw_id).strip()

    m = re.search(r"(\d{3})$", raw_id)
    if m:
        return m.group(1)

    try:
        return f"{int(float(raw_id)):03d}"
    except Exception:
        raise ValueError(f"无法解析编号：{raw_id}")


def read_name_table(table_path):
    """
    读取名字对照表。

    约定：
    第1列：编号，例如 000
    第2列：widhr1
    第3列：vocc1
    第4列：widhr2
    第5列：vocc2
    第6列：目前未使用
    """
    mapping = {}

    with open(table_path, "r", encoding="utf-8-sig") as f:
        for line_no, line in enumerate(f, start=1):
            line = line.strip()

            if not line:
                continue

            # 跳过注释行
            if line.startswith("#") or line.startswith("%"):
                continue

            # 支持空格、Tab、英文逗号、中文逗号分隔
            parts = re.split(r"[\s,，]+", line)

            if len(parts) < 5:
                print(f"[警告] 第 {line_no} 行列数不足，已跳过：{line}")
                continue

            try:
                case_id = normalize_case_id(parts[0])
            except Exception:
                # 可能是表头，直接跳过
                print(f"[提示] 第 {line_no} 行无法识别编号，可能是表头，已跳过：{line}")
                continue

            mapping[case_id] = {
                "widhr1": parts[1],
                "vocc1": parts[2],
                "widhr2": parts[3],
                "vocc2": parts[4],
            }

    return mapping


def find_boundary_files(case_folder):
    """
    查找 boundary_curve_1.txt ~ boundary_curve_X.txt，并按数字排序。
    """
    files = list(case_folder.glob("boundary_curve_*.txt"))

    def sort_key(path):
        m = re.search(r"boundary_curve_(\d+)\.txt$", path.name)
        if m:
            return int(m.group(1))
        return 10**9

    return sorted(files, key=sort_key)


def unlock_file(path, delete_existing=False):
    """
    清理可能存在的 COMSOL 锁文件。
    若 delete_existing=True，则删除已存在的旧 mph 文件。
    """
    path = Path(path)

    possible_lock_files = [
        Path(str(path) + ".lock"),
        path.with_name(path.name + ".lock"),
        path.with_name("." + path.name + ".lock"),
    ]

    for lock_file in possible_lock_files:
        if lock_file.exists():
            try:
                lock_file.unlink()
                print(f"[清理锁文件] {lock_file}")
            except Exception as e:
                print(f"[警告] 锁文件删除失败：{lock_file}，原因：{e}")

    if delete_existing and path.exists():
        try:
            path.unlink()
            print(f"[删除旧文件] {path}")
        except Exception as e:
            print(f"[警告] 旧文件删除失败：{path}，原因：{e}")


def write_log_header():
    if not LOG_CSV.exists():
        with open(LOG_CSV, "w", newline="", encoding="utf-8-sig") as f:
            writer = csv.writer(f)
            writer.writerow([
                "case_id",
                "status",
                "num_boundary_files",
                "export_txt",
                "saved_mph",
                "time_seconds",
                "message",
            ])

    if not DOMAIN_LOG_CSV.exists():
        with open(DOMAIN_LOG_CSV, "w", newline="", encoding="utf-8-sig") as f:
            writer = csv.writer(f)
            writer.writerow([
                "case_id",
                "mat2_before",
                "spf_before",
                "final_domains",
                "action",
                "message",
            ])


def append_log(case_id, status, num_boundary_files, export_txt, saved_mph, time_seconds, message):
    with open(LOG_CSV, "a", newline="", encoding="utf-8-sig") as f:
        writer = csv.writer(f)
        writer.writerow([
            case_id,
            status,
            num_boundary_files,
            str(export_txt) if export_txt else "",
            str(saved_mph) if saved_mph else "",
            f"{time_seconds:.2f}",
            message,
        ])


def append_domain_log(case_id, mat2_before, spf_before, final_domains, action, message):
    with open(DOMAIN_LOG_CSV, "a", newline="", encoding="utf-8-sig") as f:
        writer = csv.writer(f)
        writer.writerow([
            case_id,
            list(mat2_before) if mat2_before is not None else "",
            list(spf_before) if spf_before is not None else "",
            list(final_domains) if final_domains is not None else "",
            action,
            message,
        ])


def java_int_array_to_list(value):
    """
    将 Java int[] / Java collection / Python list 转为 Python int list。
    """
    if value is None:
        return None

    try:
        return [int(x) for x in list(value)]
    except Exception:
        pass

    try:
        n = len(value)
        return [int(value[i]) for i in range(n)]
    except Exception:
        pass

    try:
        return [int(value)]
    except Exception:
        return None


def unique_sorted_ints(values):
    """
    去重并排序。
    """
    if values is None:
        return None
    return sorted({int(x) for x in values})


def has_required_domains(entities):
    """
    检查是否包含 REQUIRED_DOMAINS 中的所有域。
    """
    if entities is None:
        return False
    s = set(entities)
    return all(d in s for d in REQUIRED_DOMAINS)


def format_domains(domains):
    if domains is None:
        return "None"
    return "[" + ", ".join(str(int(x)) for x in domains) + "]"


# =========================
# 5. COMSOL 操作函数
# =========================

def set_parameters(java_model, params):
    """
    修改 COMSOL 参数。
    """
    for name, value in params.items():
        java_model.param().set(name, str(value))
        print(f"    参数设置：{name} = {value}")


def remove_existing_generated_polygons(geom):
    """
    删除已有的 pol2、pol3、...，保留 pol1。
    这样每个算例都能重新按分块数量生成多边形。
    """
    try:
        tags = list(geom.feature().tags())
    except Exception:
        tags = []

    for tag in tags:
        tag = str(tag)
        if re.fullmatch(r"pol\d+", tag) and tag != "pol1":
            try:
                geom.feature().remove(tag)
                print(f"    删除旧多边形：{tag}")
            except Exception as e:
                print(f"    [警告] 删除 {tag} 失败：{e}")


def set_polygon_files(java_model, boundary_files):
    """
    按 boundary_curve 文件数量生成 pol1 ~ polX。
    pol1 使用源模型中已有的多边形；
    pol2 及以后由 pol1 复制得到。
    """
    if not boundary_files:
        raise RuntimeError("未找到 boundary_curve_*.txt 文件")

    geom = java_model.component("comp1").geom("geom1")

    remove_existing_generated_polygons(geom)

    # 第一个分块：直接使用模板中的 pol1
    first_file = str(boundary_files[0])
    geom.feature("pol1").set("filename", first_file)
    print(f"    pol1 -> {first_file}")

    # 后续分块：复制 pol1 得到 pol2 ~ polX
    for idx, boundary_file in enumerate(boundary_files[1:], start=2):
        pol_tag = f"pol{idx}"

        geom.feature().duplicate(pol_tag, "pol1")
        geom.feature(pol_tag).set("filename", str(boundary_file))

        print(f"    {pol_tag} -> {boundary_file}")


def build_geometry_and_mesh(java_model):
    """
    构建几何并划分网格。
    """
    geom = java_model.component("comp1").geom("geom1")

    try:
        geom.runPre("fin")
    except Exception:
        # 某些模型里 runPre("fin") 不可用时，用 geom.run() 兜底
        print("    [提示] geom.runPre('fin') 失败，改用 geom.run()")
        geom.run()

    java_model.component("comp1").mesh("mesh1").run()


def read_selection_entities_by_api(java_model):
    """
    直接用 COMSOL Java API 读取 mat2 和 spf 的 selection entities。
    若 API 在当前 COMSOL/mph 环境中不可用，会抛异常，由上层转 XML 兜底。
    """
    mat2_raw = java_model.component("comp1").material("mat2").selection().entities()
    spf_raw = java_model.component("comp1").physics("spf").selection().entities()

    mat2_entities = unique_sorted_ints(java_int_array_to_list(mat2_raw))
    spf_entities = unique_sorted_ints(java_int_array_to_list(spf_raw))

    return mat2_entities, spf_entities


def parse_comsol_entities_string(entities_string):
    """
    解析 dmodel.xml 中类似 entities="3,1,3,4,6" 的字段。
    check_spf_domains.py 中第一个数是 COMSOL 内部编码头，这里同样跳过。
    """
    if not entities_string:
        return None

    parts = [p.strip() for p in entities_string.split(",") if p.strip()]
    nums = []
    for p in parts:
        try:
            nums.append(int(p))
        except Exception:
            pass

    if not nums:
        return None

    # 按原 check_spf_domains.py 的规则，跳过第一个内部编码头。
    # 若某些 XML 版本没有头，此处可通过后续“必须包含 1 和 3”的检查发现异常。
    if len(nums) >= 2:
        nums = nums[1:]

    return unique_sorted_ints(nums)


def read_dmodel_xml_from_mph(mph_path):
    """
    .mph 通常是 ZIP 包，里面有 dmodel.xml。
    这里不固定 dmodel.xml 的目录位置，而是在 zip 内部搜索。
    """
    with zipfile.ZipFile(mph_path, "r") as z:
        names = z.namelist()

        xml_names = [n for n in names if n.endswith("dmodel.xml")]
        if not xml_names:
            raise RuntimeError(f"mph 内未找到 dmodel.xml：{mph_path}")

        xml_name = xml_names[0]
        return z.read(xml_name).decode("utf-8", errors="ignore")


def extract_entities_from_xml_block(xml, block_name, tag):
    """
    从指定 XML 块中提取 entities。
    例如：
    - block_name='Physics', tag='spf'
    - block_name='Material', tag='mat2'

    说明：
    - spf 的解析方式与 check_spf_domains.py 一致；
    - mat2 在不同 COMSOL 版本中 XML 结构可能不同，因此加了“tag 附近窗口”的兜底。
    """
    tag_re = re.escape(tag)
    block_re = re.compile(
        rf'<{block_name}\b[^>]*\btag="{tag_re}"[^>]*>.*?</{block_name}>',
        re.DOTALL
    )
    match = block_re.search(xml)

    blocks = []
    if match:
        blocks.append(match.group())

    # 处理自闭合标签，例如 <Material ... />
    self_close_re = re.compile(
        rf'<{block_name}\b[^>]*\btag="{tag_re}"[^>]*/>',
        re.DOTALL
    )
    match2 = self_close_re.search(xml)
    if match2:
        blocks.append(match2.group())

    # 最后兜底：截取 tag 附近片段，避免 Material 嵌套格式不完全匹配。
    if not blocks:
        idx = xml.find(f'tag="{tag}"')
        if idx >= 0:
            blocks.append(xml[max(0, idx - 1000): idx + 5000])

    for block in blocks:
        ent_matches = re.findall(r'\bentities="([^"]+)"', block)
        for ent in ent_matches:
            parsed = parse_comsol_entities_string(ent)
            if parsed:
                return parsed

    return None


def read_selection_entities_by_temp_mph_xml(model, case_id):
    """
    Java API 读取失败时的兜底方案：
    1. 临时保存当前模型为 mph
    2. 从 dmodel.xml 中解析 mat2 / spf 的 entities
    3. 默认删除临时 mph
    """
    tmp_mph = DOMAIN_CHECK_TMP_DIR / f"_domain_check_{case_id}.mph"

    unlock_file(tmp_mph, delete_existing=True)
    model.save(str(tmp_mph))

    try:
        xml = read_dmodel_xml_from_mph(tmp_mph)

        mat2_entities = extract_entities_from_xml_block(xml, "Material", "mat2")
        spf_entities = extract_entities_from_xml_block(xml, "Physics", "spf")

        return unique_sorted_ints(mat2_entities), unique_sorted_ints(spf_entities)
    finally:
        if not KEEP_TEMP_DOMAIN_CHECK_MPH:
            unlock_file(tmp_mph, delete_existing=True)


def get_current_mat2_spf_domains(java_model, model, case_id):
    """
    获取 mat2 和 spf 当前选中域。
    优先 Java API，失败则临时保存 mph 并解析 dmodel.xml。
    """
    api_error = None

    try:
        mat2_entities, spf_entities = read_selection_entities_by_api(java_model)

        if mat2_entities is not None and spf_entities is not None:
            print(f"    [域检查/API] mat2={format_domains(mat2_entities)}, spf={format_domains(spf_entities)}")
            return mat2_entities, spf_entities, "api"

    except Exception as e:
        api_error = e
        print(f"    [提示] Java API 读取域失败，准备使用临时 mph XML 兜底：{e}")

    if USE_TEMP_MPH_XML_FALLBACK:
        mat2_entities, spf_entities = read_selection_entities_by_temp_mph_xml(model, case_id)
        print(f"    [域检查/XML] mat2={format_domains(mat2_entities)}, spf={format_domains(spf_entities)}")
        return mat2_entities, spf_entities, "xml"

    raise RuntimeError(f"无法读取 mat2/spf 选中域。Java API 错误：{api_error}")


def decide_final_domains(mat2_entities, spf_entities):
    """
    根据文档第4步规则，决定是否修正以及最终域列表。

    正确情况：
    - mat2 和 spf 选中域完全一致
    - 数量为 EXPECTED_DOMAIN_COUNT
    - 包含 REQUIRED_DOMAINS，即 1 和 3

    修正情况：
    - 任一模块超过三个域
    - mat2 与 spf 不一致
    - 数量不是三个域
    修正后：保留 1、3、最大域号。
    """
    if mat2_entities is None:
        raise RuntimeError("未能读取材料 mat2 的选中域")
    if spf_entities is None:
        raise RuntimeError("未能读取层流 spf 的选中域")

    mat2_entities = unique_sorted_ints(mat2_entities)
    spf_entities = unique_sorted_ints(spf_entities)

    if STRICT_REQUIRED_DOMAINS:
        if not has_required_domains(mat2_entities):
            raise RuntimeError(f"mat2 当前选中域不包含必须域 {REQUIRED_DOMAINS}：{mat2_entities}")
        if not has_required_domains(spf_entities):
            raise RuntimeError(f"spf 当前选中域不包含必须域 {REQUIRED_DOMAINS}：{spf_entities}")

    already_ok = (
        mat2_entities == spf_entities
        and len(mat2_entities) == EXPECTED_DOMAIN_COUNT
        and has_required_domains(mat2_entities)
    )

    if already_ok:
        return mat2_entities, False, "当前 mat2/spf 域选择正确，无需修正"

    union_domains = unique_sorted_ints((mat2_entities or []) + (spf_entities or []))

    if STRICT_REQUIRED_DOMAINS and not has_required_domains(union_domains):
        raise RuntimeError(
            f"mat2/spf 合并后仍不包含必须域 {REQUIRED_DOMAINS}，无法安全修正：{union_domains}"
        )

    extra_domains = [d for d in union_domains if d not in set(REQUIRED_DOMAINS)]

    if not extra_domains:
        if STRICT_REQUIRED_DOMAINS:
            raise RuntimeError(
                f"除必须域 {REQUIRED_DOMAINS} 外没有可作为 X 的域，无法形成 1、3、X：{union_domains}"
            )
        final_domains = unique_sorted_ints(REQUIRED_DOMAINS)
    else:
        # 文档规则：保留 1、3 和最大值
        final_domains = unique_sorted_ints(REQUIRED_DOMAINS + [max(extra_domains)])

    msg = (
        f"需要修正：mat2={format_domains(mat2_entities)}, "
        f"spf={format_domains(spf_entities)} -> final={format_domains(final_domains)}"
    )
    return final_domains, True, msg


def set_selection_entities(selection_obj, domains):
    """
    给 COMSOL selection 设置域。
    常见写法是 selection().set(1, 3, 6)。
    不同 mph/JPype 版本对可变参数和 list 的支持略有差异，因此做多种尝试。
    """
    domains = [int(x) for x in domains]

    # 1) varargs: set(1, 3, 6)
    try:
        selection_obj.set(*domains)
        return
    except Exception as e1:
        err1 = e1

    # 2) list: set([1, 3, 6])
    try:
        selection_obj.set(domains)
        return
    except Exception as e2:
        err2 = e2

    # 3) tuple: set((1, 3, 6))
    try:
        selection_obj.set(tuple(domains))
        return
    except Exception as e3:
        raise RuntimeError(
            f"selection.set 设置失败，domains={domains}；"
            f"varargs错误={err1}；list错误={err2}；tuple错误={e3}"
        )


def apply_mat2_spf_domains(java_model, domains):
    """
    将最终域列表同时写回材料 mat2 和层流 spf。
    """
    domains = [int(x) for x in domains]

    mat2_sel = java_model.component("comp1").material("mat2").selection()
    spf_sel = java_model.component("comp1").physics("spf").selection()

    set_selection_entities(mat2_sel, domains)
    set_selection_entities(spf_sel, domains)

    print(f"    已设置 mat2 域：{domains}")
    print(f"    已设置 spf  域：{domains}")


def check_and_fix_mat2_spf_domains(java_model, model, case_id):
    """
    文档第4步：检查并取消多余的域。
    插入位置：构建几何与网格之后、sol1 运行之前。
    """
    if not CHECK_AND_FIX_DOMAINS:
        append_domain_log(case_id, "", "", "", "DISABLED", "未启用域检查")
        return "DISABLED", "未启用域检查"

    mat2_before, spf_before, read_mode = get_current_mat2_spf_domains(java_model, model, case_id)
    final_domains, need_fix, msg = decide_final_domains(mat2_before, spf_before)

    if need_fix:
        apply_mat2_spf_domains(java_model, final_domains)
        action = "FIXED"
    else:
        action = "OK"

    log_msg = f"{msg}; read_mode={read_mode}"
    append_domain_log(case_id, mat2_before, spf_before, final_domains, action, log_msg)

    print(f"    [域检查结果] {action}: {log_msg}")
    return action, log_msg


def run_solution(java_model):
    """
    运行 sol1。
    """
    java_model.sol("sol1").runAll()


def export_temperature(java_model, export_txt):
    """
    导出温度场 TXT。
    """
    java_model.result().export("data1").set("filename", str(export_txt))
    java_model.result().export("data1").run()


def close_model(client, model):
    """
    尝试从 COMSOL server 中移除模型，减少内存占用。
    不同 mph 版本接口可能略有差异，所以这里做容错。
    """
    try:
        client.remove(model)
    except Exception:
        pass


# =========================
# 6. 单个算例处理
# =========================

def process_one_case(client, case_id, param_mapping):
    case_id = normalize_case_id(case_id)
    t0 = time.time()

    case_folder = BOUNDARY_ROOT / f"comsol_topology_{case_id}"
    export_txt = EXPORT_ROOT / f"模型结构导入comsol仿真温度场_{case_id}.txt"
    save_mph_path = MPH_SAVE_DIR / f"模型结构导入comsol再仿真_{case_id}.mph"

    model = None
    domain_action = ""
    domain_msg = ""

    try:
        print("\n" + "=" * 80)
        print(f"[开始] comsol_topology_{case_id}")

        if not case_folder.exists():
            raise FileNotFoundError(f"拓扑文件夹不存在：{case_folder}")

        if case_id not in param_mapping:
            raise KeyError(f"名字对照表中找不到编号 {case_id}")

        if SKIP_IF_EXPORT_EXISTS and export_txt.exists():
            msg = "温度场 TXT 已存在，跳过"
            print(f"[跳过] {msg}")
            append_log(case_id, "SKIPPED", 0, export_txt, "", time.time() - t0, msg)
            return

        boundary_files = find_boundary_files(case_folder)

        if not boundary_files:
            raise FileNotFoundError(f"未找到 boundary_curve_*.txt：{case_folder}")

        print(f"    分块数量：{len(boundary_files)}")

        # 每次都重新打开干净源模型
        print(f"    加载源 mph：{MPH_SOURCE}")
        model = client.load(str(MPH_SOURCE))
        java_model = model.java

        # 修改参数
        params = param_mapping[case_id]
        set_parameters(java_model, params)

        # 设置多边形文件
        set_polygon_files(java_model, boundary_files)

        # 构建几何与网格
        print("    构建几何与划分网格...")
        build_geometry_and_mesh(java_model)

        # 文档第4步：检查并取消多余的域
        print("    检查 mat2/spf 域并取消多余域...")
        domain_action, domain_msg = check_and_fix_mat2_spf_domains(java_model, model, case_id)

        # 仿真
        print("    运行 sol1...")
        run_solution(java_model)

        # 导出温度场
        print(f"    导出温度场：{export_txt}")
        export_temperature(java_model, export_txt)

        # 保存 mph
        saved_mph = ""
        if SAVE_MPH:
            unlock_file(save_mph_path, delete_existing=DELETE_OLD_MPH)
            print(f"    保存 mph：{save_mph_path}")
            model.save(str(save_mph_path))
            saved_mph = save_mph_path

        cost = time.time() - t0
        msg = f"完成；domain_action={domain_action}；{domain_msg}"
        print(f"[完成] comsol_topology_{case_id}，耗时 {cost:.2f} 秒")

        append_log(
            case_id=case_id,
            status="OK",
            num_boundary_files=len(boundary_files),
            export_txt=export_txt,
            saved_mph=saved_mph,
            time_seconds=cost,
            message=msg,
        )

    except Exception as e:
        cost = time.time() - t0
        err_msg = str(e)
        print(f"[失败] comsol_topology_{case_id}")
        print(err_msg)
        traceback.print_exc()

        append_log(
            case_id=case_id,
            status="FAILED",
            num_boundary_files=0,
            export_txt=export_txt,
            saved_mph="",
            time_seconds=cost,
            message=err_msg,
        )

        # 若在域检查之前失败，也记一条域日志，方便排查。
        if not domain_action:
            append_domain_log(case_id, "", "", "", "FAILED", err_msg)

    finally:
        if model is not None:
            close_model(client, model)


# =========================
# 7. 主程序
# =========================

def main():
    ensure_dirs()
    write_log_header()

    if not MPH_SOURCE.exists():
        raise FileNotFoundError(f"源 mph 文件不存在：{MPH_SOURCE}")

    if not BOUNDARY_ROOT.exists():
        raise FileNotFoundError(f"边界点根目录不存在：{BOUNDARY_ROOT}")

    if not NAME_TABLE.exists():
        raise FileNotFoundError(f"名字对照表不存在：{NAME_TABLE}")

    print("[读取名字对照表]")
    param_mapping = read_name_table(NAME_TABLE)
    print(f"    成功读取 {len(param_mapping)} 条参数记录")

    print("[启动 COMSOL]")
    client = mph.start()

    for i in range(START_ID, END_ID + 1):
        case_id = f"{i:03d}"
        process_one_case(client, case_id, param_mapping)

    print("\n" + "=" * 80)
    print("[全部结束]")
    print(f"日志文件：{LOG_CSV}")
    print(f"域检查日志：{DOMAIN_LOG_CSV}")


if __name__ == "__main__":
    main()
