# -*- coding: utf-8 -*-
"""
批量可视化拓扑结构 TXT 文件，并保存为 PNG

功能：
1. 遍历目录 D:\生成式拓扑优化HR\数据集设计\2-comsol拓扑优化批量生成\OUT\02_out拓扑结构 下所有 .txt
2. 读取三列数据 x, y, value
3. 参考给定示例代码，使用 griddata + pcolormesh 进行可视化
4. 将图片批量保存到 D:\生成式拓扑优化HR\数据集设计\2-comsol拓扑优化批量生成\OUT\可视化
"""

import os
from pathlib import Path

import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import griddata


# =========================
# 路径配置
# =========================
INPUT_DIR = Path(r"D:\Diff_Topo\开源\topodiff-Evaluate\1564dataset\3批量导入comsol再仿真\再仿真温度场")
OUTPUT_DIR = Path(r"D:\Diff_Topo\开源\topodiff-Evaluate\1564dataset\3批量导入comsol再仿真\再仿真温度场\可视化")

# 网格分辨率，可按需调整
GRID_RESOLUTION = 500

# 插值方式：nearest / linear / cubic
INTERP_METHOD = "nearest"

# 图片 DPI
SAVE_DPI = 200

# 是否显示颜色条
SHOW_COLORBAR = True

# 颜色映射
CMAP = "jet"


def visualize_one_txt(txt_path: Path, output_dir: Path) -> None:
    """将单个 TXT 文件可视化并保存为 PNG。"""
    data = np.loadtxt(txt_path, comments='%', encoding='utf-8')

    if data.ndim != 2 or data.shape[1] < 3:
        raise ValueError(f"文件格式错误，至少需要三列数据：{txt_path}")

    x = data[:, 0]
    y = data[:, 1]
    value = data[:, 2]

    # 创建规则网格
    xi = np.linspace(x.min(), x.max(), GRID_RESOLUTION)
    yi = np.linspace(y.min(), y.max(), GRID_RESOLUTION)
    xi, yi = np.meshgrid(xi, yi)

    # 网格化插值
    zi = griddata((x, y), value, (xi, yi), method=INTERP_METHOD)

    # 对于 linear/cubic 可能产生 NaN，补成最近邻结果
    if np.isnan(zi).any():
        zi_nearest = griddata((x, y), value, (xi, yi), method="nearest")
        zi = np.where(np.isnan(zi), zi_nearest, zi)

    # 生成输出文件名
    out_name = txt_path.stem + ".png"
    out_path = output_dir / out_name

    # 画图
    plt.figure(figsize=(8, 6))
    mesh = plt.pcolormesh(xi, yi, zi, cmap=CMAP, shading='auto')

    if SHOW_COLORBAR:
        cbar = plt.colorbar(mesh)
        cbar.set_label('Value')

    plt.xlabel('x')
    plt.ylabel('y')
    plt.title(txt_path.stem)
    plt.axis('equal')
    plt.tight_layout()

    plt.savefig(out_path, dpi=SAVE_DPI, bbox_inches='tight')
    plt.close()

    print(f"[OK] 已保存: {out_path}")


def main() -> None:
    if not INPUT_DIR.exists():
        raise FileNotFoundError(f"输入目录不存在：{INPUT_DIR}")

    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)

    txt_files = sorted(INPUT_DIR.glob("*.txt"))
    if not txt_files:
        print(f"[提示] 未在目录中找到 TXT 文件：{INPUT_DIR}")
        return

    print("=" * 80)
    print(f"输入目录: {INPUT_DIR}")
    print(f"输出目录: {OUTPUT_DIR}")
    print(f"TXT 文件数量: {len(txt_files)}")
    print(f"插值方式: {INTERP_METHOD}")
    print(f"网格分辨率: {GRID_RESOLUTION}")
    print("=" * 80)

    success_count = 0
    fail_count = 0

    for idx, txt_path in enumerate(txt_files, start=1):
        print(f"\n[{idx}/{len(txt_files)}] 正在处理: {txt_path.name}")
        try:
            visualize_one_txt(txt_path, OUTPUT_DIR)
            success_count += 1
        except Exception as e:
            fail_count += 1
            print(f"[失败] {txt_path.name} -> {e}")

    print("\n" + "=" * 80)
    print("处理完成")
    print(f"成功: {success_count}")
    print(f"失败: {fail_count}")
    print(f"输出目录: {OUTPUT_DIR}")
    print("=" * 80)


if __name__ == "__main__":
    main()
