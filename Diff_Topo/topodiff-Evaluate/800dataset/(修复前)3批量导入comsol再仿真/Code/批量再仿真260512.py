# -*- coding: utf-8 -*-
"""
批量导入 COMSOL 边界点并重新仿真

功能流程：
1. 每次从干净的源 mph 文件重新打开模型
2. 根据名字对照表修改 widhr1、vocc1、widhr2、vocc2
3. 遍历 comsol_topology_000 ~ comsol_topology_099
4. 读取每个文件夹下 boundary_curve_1.txt ~ boundary_curve_X.txt
5. 自动生成 COMSOL 中的 pol1 ~ polX
6. 构建几何、划分网格、运行 sol1
7. 导出温度场 TXT
8. 另存 mph 文件，不污染源 mph
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
    r"D:\Diff_Topo\开源\topodiff-Evaluate\3批量导入comsol再仿真\模型结构导入comsolBench.mph"
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
# 3. 工具函数
# =========================

def ensure_dirs():
    EXPORT_ROOT.mkdir(parents=True, exist_ok=True)
    MPH_SAVE_DIR.mkdir(parents=True, exist_ok=True)


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
            "export_txt",
            "saved_mph",
            "time_seconds",
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
# 5. 单个算例处理
# =========================

def process_one_case(client, case_id, param_mapping):
    case_id = normalize_case_id(case_id)
    t0 = time.time()

    case_folder = BOUNDARY_ROOT / f"comsol_topology_{case_id}"
    export_txt = EXPORT_ROOT / f"模型结构导入comsol仿真温度场_{case_id}.txt"
    save_mph_path = MPH_SAVE_DIR / f"模型结构导入comsol再仿真_{case_id}.mph"

    model = None

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
        msg = "完成"
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


if __name__ == "__main__":
    main()