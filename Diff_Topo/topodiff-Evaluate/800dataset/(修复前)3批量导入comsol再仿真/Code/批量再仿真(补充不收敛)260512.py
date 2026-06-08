# -*- coding: utf-8 -*-
"""
COMSOL 批量补跑脚本

功能：
1. 检查原始温度场输出目录中：
   模型结构导入comsol仿真温度场_000.txt ~ 模型结构导入comsol仿真温度场_099.txt
   缺失了哪些编号。

2. 只对缺失编号重新执行完整流程：
   重新打开干净源 mph -> 改参数 -> 导入多边形 -> 构建几何 -> 划分网格 -> 运行 sol1 -> 导出温度场 -> 保存 mph

3. 补跑结果保存到：
   TXT:
   D:\Diff_Topo\开源\topodiff-Evaluate\3批量导入comsol再仿真\再仿真温度场\再仿真温度场(补充不收敛)\txt

   MPH:
   D:\Diff_Topo\开源\topodiff-Evaluate\3批量导入comsol再仿真\再仿真温度场\再仿真温度场(补充不收敛)\mph
"""

from pathlib import Path
import re
import csv
import time
import traceback

import mph


# =========================
# 1. 路径配置
# =========================

MPH_SOURCE = Path(
    r"D:\Diff_Topo\开源\topodiff-Evaluate\3批量导入comsol再仿真\模型结构导入comsolBench极细.mph"
)

BOUNDARY_ROOT = Path(
    r"D:\Diff_Topo\开源\topodiff-Evaluate\2分块输出边界点"
)

NAME_TABLE = Path(
    r"D:\Diff_Topo\开源\topodiff-Evaluate\1三列数据二值化\名字对照表.txt"
)

# 原始温度场输出目录：用来检查哪些编号缺失
ORIGINAL_EXPORT_ROOT = Path(
    r"D:\Diff_Topo\开源\topodiff-Evaluate\3批量导入comsol再仿真\再仿真温度场"
)

# 补跑输出目录
SUPPLEMENT_ROOT = ORIGINAL_EXPORT_ROOT / "再仿真温度场(补充不收敛)"
SUPPLEMENT_TXT_DIR = SUPPLEMENT_ROOT / "txt"
SUPPLEMENT_MPH_DIR = SUPPLEMENT_ROOT / "mph"

LOG_CSV = SUPPLEMENT_ROOT / "batch_comsol_missing_log.csv"


# =========================
# 2. 批量范围配置
# =========================

START_ID = 0
END_ID = 99

# 判断文件是否有效的最小字节数。
# 如果只想判断“文件是否存在”，设为 0。
# 设为 1 时，空文件也会被当成无效，继续补跑。
MIN_VALID_EXPORT_BYTES = 1

# 如果补跑目录中已经有同名 TXT，是否跳过。
# True：补跑目录已有就不再跑。
# False：即使补跑目录已有，也重新覆盖跑一遍。
SKIP_IF_SUPPLEMENT_EXISTS = False

# 是否保存补跑的 mph
SAVE_MPH = True

# 保存补跑 mph 前，如果已有同名 mph，是否删除旧文件
DELETE_OLD_MPH = True


# =========================
# 3. 工具函数
# =========================

def ensure_dirs():
    SUPPLEMENT_ROOT.mkdir(parents=True, exist_ok=True)
    SUPPLEMENT_TXT_DIR.mkdir(parents=True, exist_ok=True)
    SUPPLEMENT_MPH_DIR.mkdir(parents=True, exist_ok=True)


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


def get_original_export_txt(case_id):
    case_id = normalize_case_id(case_id)
    return ORIGINAL_EXPORT_ROOT / f"模型结构导入comsol仿真温度场_{case_id}.txt"


def get_supplement_export_txt(case_id):
    case_id = normalize_case_id(case_id)
    return SUPPLEMENT_TXT_DIR / f"模型结构导入comsol仿真温度场_{case_id}.txt"


def get_supplement_mph_path(case_id):
    case_id = normalize_case_id(case_id)
    return SUPPLEMENT_MPH_DIR / f"模型结构导入comsol再仿真_{case_id}.mph"


def is_valid_export_file(path):
    """
    判断原始温度场 TXT 是否有效。
    默认逻辑：
    1. 文件必须存在；
    2. 文件大小必须大于 MIN_VALID_EXPORT_BYTES。
    """
    path = Path(path)

    if not path.exists():
        return False

    if MIN_VALID_EXPORT_BYTES > 0:
        try:
            if path.stat().st_size < MIN_VALID_EXPORT_BYTES:
                return False
        except Exception:
            return False

    return True


def find_missing_case_ids():
    """
    检查原始输出目录中缺失的编号。
    """
    missing_ids = []

    for i in range(START_ID, END_ID + 1):
        case_id = f"{i:03d}"
        original_txt = get_original_export_txt(case_id)

        if not is_valid_export_file(original_txt):
            missing_ids.append(case_id)

    return missing_ids


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
            print(f"[删除旧 mph] {path}")
        except Exception as e:
            print(f"[警告] 旧 mph 删除失败：{path}，原因：{e}")


def write_log_header():
    if LOG_CSV.exists():
        return

    with open(LOG_CSV, "w", newline="", encoding="utf-8-sig") as f:
        writer = csv.writer(f)
        writer.writerow([
            "case_id",
            "status",
            "num_boundary_files",
            "original_txt",
            "supplement_txt",
            "supplement_mph",
            "time_seconds",
            "message",
        ])


def append_log(
    case_id,
    status,
    num_boundary_files,
    original_txt,
    supplement_txt,
    supplement_mph,
    time_seconds,
    message,
):
    with open(LOG_CSV, "a", newline="", encoding="utf-8-sig") as f:
        writer = csv.writer(f)
        writer.writerow([
            case_id,
            status,
            num_boundary_files,
            str(original_txt) if original_txt else "",
            str(supplement_txt) if supplement_txt else "",
            str(supplement_mph) if supplement_mph else "",
            f"{time_seconds:.2f}",
            message,
        ])


# =========================
# 4. COMSOL 操作函数
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

    first_file = str(boundary_files[0])
    geom.feature("pol1").set("filename", first_file)
    print(f"    pol1 -> {first_file}")

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
        print("    [提示] geom.runPre('fin') 失败，改用 geom.run()")
        geom.run()

    java_model.component("comp1").mesh("mesh1").run()


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
    """
    try:
        client.remove(model)
    except Exception:
        pass


# =========================
# 5. 单个缺失算例补跑
# =========================

def process_one_missing_case(client, case_id, param_mapping):
    case_id = normalize_case_id(case_id)
    t0 = time.time()

    case_folder = BOUNDARY_ROOT / f"comsol_topology_{case_id}"

    original_txt = get_original_export_txt(case_id)
    supplement_txt = get_supplement_export_txt(case_id)
    supplement_mph = get_supplement_mph_path(case_id)

    model = None
    num_boundary_files = 0

    try:
        print("\n" + "=" * 80)
        print(f"[补跑开始] comsol_topology_{case_id}")
        print(f"    原始缺失文件：{original_txt}")
        print(f"    补跑 TXT 输出：{supplement_txt}")
        print(f"    补跑 MPH 输出：{supplement_mph}")

        if not case_folder.exists():
            raise FileNotFoundError(f"拓扑文件夹不存在：{case_folder}")

        if case_id not in param_mapping:
            raise KeyError(f"名字对照表中找不到编号 {case_id}")

        if SKIP_IF_SUPPLEMENT_EXISTS and supplement_txt.exists():
            msg = "补跑目录中已经存在同名 TXT，跳过"
            print(f"[跳过] {msg}")
            append_log(
                case_id,
                "SKIPPED",
                0,
                original_txt,
                supplement_txt,
                supplement_mph,
                time.time() - t0,
                msg,
            )
            return

        boundary_files = find_boundary_files(case_folder)
        num_boundary_files = len(boundary_files)

        if not boundary_files:
            raise FileNotFoundError(f"未找到 boundary_curve_*.txt：{case_folder}")

        print(f"    分块数量：{num_boundary_files}")

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

        # 仿真
        print("    运行 sol1...")
        run_solution(java_model)

        # 导出温度场
        print(f"    导出补跑温度场：{supplement_txt}")
        export_temperature(java_model, supplement_txt)

        # 保存 mph
        saved_mph = ""
        if SAVE_MPH:
            unlock_file(supplement_mph, delete_existing=DELETE_OLD_MPH)
            print(f"    保存补跑 mph：{supplement_mph}")
            model.save(str(supplement_mph))
            saved_mph = supplement_mph

        cost = time.time() - t0
        msg = "补跑完成"
        print(f"[补跑完成] comsol_topology_{case_id}，耗时 {cost:.2f} 秒")

        append_log(
            case_id=case_id,
            status="OK",
            num_boundary_files=num_boundary_files,
            original_txt=original_txt,
            supplement_txt=supplement_txt,
            supplement_mph=saved_mph,
            time_seconds=cost,
            message=msg,
        )

    except Exception as e:
        cost = time.time() - t0
        err_msg = str(e)
        print(f"[补跑失败] comsol_topology_{case_id}")
        print(err_msg)
        traceback.print_exc()

        append_log(
            case_id=case_id,
            status="FAILED",
            num_boundary_files=num_boundary_files,
            original_txt=original_txt,
            supplement_txt=supplement_txt,
            supplement_mph="",
            time_seconds=cost,
            message=err_msg,
        )

    finally:
        if model is not None:
            close_model(client, model)


# =========================
# 6. 主程序
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

    if not ORIGINAL_EXPORT_ROOT.exists():
        raise FileNotFoundError(f"原始温度场输出目录不存在：{ORIGINAL_EXPORT_ROOT}")

    print("[1] 检查原始温度场 TXT 缺失情况")
    missing_ids = find_missing_case_ids()

    if not missing_ids:
        print("没有发现缺失文件，000~099 已全部存在。")
        return

    print(f"发现缺失数量：{len(missing_ids)}")
    print("缺失编号如下：")
    print(", ".join(missing_ids))

    print("\n[2] 读取名字对照表")
    param_mapping = read_name_table(NAME_TABLE)
    print(f"    成功读取 {len(param_mapping)} 条参数记录")

    print("\n[3] 启动 COMSOL，开始补跑缺失编号")
    client = mph.start()

    for case_id in missing_ids:
        process_one_missing_case(client, case_id, param_mapping)

    print("\n" + "=" * 80)
    print("[补跑任务结束]")
    print(f"补跑 TXT 输出目录：{SUPPLEMENT_TXT_DIR}")
    print(f"补跑 MPH 输出目录：{SUPPLEMENT_MPH_DIR}")
    print(f"补跑日志文件：{LOG_CSV}")


if __name__ == "__main__":
    main()