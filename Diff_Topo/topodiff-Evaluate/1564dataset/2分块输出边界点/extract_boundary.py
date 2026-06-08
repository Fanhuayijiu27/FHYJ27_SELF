import numpy as np
import os
import glob
import shutil
import matplotlib.pyplot as plt

# Configuration
input_dir = r'D:\Diff_Topo\开源\topodiff-Evaluate\1564dataset\1三列数据二值化\comsol_topology_XXX'

output_base_dir = r'D:\Diff_Topo\开源\topodiff-Evaluate\1564dataset\2分块输出边界点'

os.makedirs(output_base_dir, exist_ok=True)

# Copy the script itself to the output folder
current_script_path = os.path.abspath(__file__)
target_script_path = os.path.join(output_base_dir, os.path.basename(current_script_path))
if current_script_path != target_script_path:
    shutil.copy(current_script_path, target_script_path)

txt_files = sorted(glob.glob(os.path.join(input_dir, 'comsol_topology_*.txt')))
print(f"Found {len(txt_files)} topology files to process.")

for filepath in txt_files:
    filename = os.path.basename(filepath)
    folder_name = os.path.splitext(filename)[0] # e.g. comsol_topology_000
    
    out_dir = os.path.join(output_base_dir, folder_name)
    os.makedirs(out_dir, exist_ok=True)
    
    data = np.loadtxt(filepath, comments='%')
    if data.shape[0] != 4096:
        print(f"Skipping {filename}: unexpected grid size.")
        continue
        
    x_grid = data[:, 0].reshape((64, 64))
    y_grid = data[:, 1].reshape((64, 64))
    val_grid = data[:, 2].astype(int).reshape((64, 64))
    
    # 1. 获取物理边界极值，用于最后的“液压钳”吸附
    x_min, x_max = np.min(x_grid), np.max(x_grid)
    y_min, y_max = np.min(y_grid), np.max(y_grid)
    
    # 2. 计算网格步长 (x 沿轴 0 变化，y 沿轴 1 变化)
    dx = x_grid[1, 0] - x_grid[0, 0]
    dy = y_grid[0, 1] - y_grid[0, 0]
    
    # 3. 人工外扩一圈空气墙 (Zero-Padding)，网格变为 66x66
    padded_val = np.pad(val_grid, 1, mode='constant', constant_values=0)
    
    # 坐标系同步完美顺延外扩一格
    padded_x = np.pad(x_grid, 1, mode='edge')
    padded_x[0, :] -= dx
    padded_x[-1, :] += dx
    
    padded_y = np.pad(y_grid, 1, mode='edge')
    padded_y[:, 0] -= dy
    padded_y[:, -1] += dy
    
    # === 核心重构：在 Padding 后的网格上提取闭合多边形 ===
    fig, ax = plt.subplots(figsize=(4, 4))
    cs = ax.contour(padded_x, padded_y, padded_val, levels=[0.5])
    
    polygons = []
    for p in cs.get_paths():
        polygons.extend(p.to_polygons())
        
    plt.close(fig) # 提取完立刻关闭，防止爆内存

    valid_idx = 1
    for poly in polygons:
        # === 第二步：边界液压钳 ===
        # 将外扩出界的点强行拍平在绝对物理边界上
        curve_x = np.clip(poly[:, 0], x_min, x_max)
        curve_y = np.clip(poly[:, 1], y_min, y_max)
        
        # 过滤掉偶尔出现的只有两个点的退化微小多边形
        if len(curve_x) < 4:
            continue
            
        out_path = os.path.join(out_dir, f'boundary_curve_{valid_idx}.txt')
        np.savetxt(out_path, np.column_stack((curve_x, curve_y)), fmt='%f', header='x y', comments='% ')
        
        # 画图逻辑
        plt.figure(figsize=(4, 4))
        plt.plot(curve_x, curve_y, '-r', linewidth=2)       
        plt.scatter(curve_x, curve_y, c='blue', s=5)        
        plt.title(f'Curve {valid_idx} ({len(curve_x)} points)')
        plt.axis('equal')
        plt.xlim(-0.005, 0.070)
        plt.ylim(-0.040, 0.040)
        
        img_path = os.path.join(out_dir, f'boundary_curve_{valid_idx}.png')
        plt.savefig(img_path, dpi=150, bbox_inches='tight')
        plt.close()
        
        valid_idx += 1
        
    print(f"Processed {filename}: extracted and plotted {valid_idx - 1} distinct closed polygons.")

print(f"\n===== All 200 samples successfully processed and split into {output_base_dir} =====")
