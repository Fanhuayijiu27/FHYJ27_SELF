import numpy as np
import os
import glob
import shutil

# 定义路径
npz_path = r'd:\Diff_Topo\开源\topodiff-main\Generate_results\generated_test100_eval\generated_flow_channels.npz'
out_dir = r'd:\Diff_Topo\开源\topodiff-Evaluate\1三列数据二值化'

# 创建输出目录
os.makedirs(out_dir, exist_ok=True)

# 复制本脚本自身到目标文件夹（根据你的要求：所有生成的文件及代码放入该文件夹）
current_script_path = os.path.abspath(__file__)
target_script_path = os.path.join(out_dir, os.path.basename(current_script_path))
if current_script_path != target_script_path:
    shutil.copy(current_script_path, target_script_path)

print(f"Loading data from {npz_path}...")
data = np.load(npz_path)
images = data['images'] # shape [100, 1, 64, 64]
conditions = data['conditions'] # shape [100, 4, 64, 64]

# 借用原始 COMSOL TXT 里的物理坐标 (X, Y)
comsol_txt_path = glob.glob(r'd:\Diff_Topo\开源\900数据补齐\out\02_out拓扑结构_二值化\*.txt')[0]
original_data = np.loadtxt(comsol_txt_path, comments='%', encoding='utf-8')
x_coords = original_data[:, 0].copy()
y_coords = original_data[:, 1].copy()

# 应用之前的坐标边缘强制吸附规则
x_coords[np.abs(x_coords - 0.0005) < 1e-5] = 0.0
x_coords[np.abs(x_coords - 0.0635) < 1e-5] = 0.064

summary_lines = ["Sample_ID\tw1(pixels)\tc1(pixels)\tw2(pixels)\tc2(pixels)\tvf(Fraction)"]

for i in range(images.shape[0]):
    # ==========================
    # 1. 二值化并保存 3 列格式 TXT
    # ==========================
    img = images[i, 0]
    binary_img = (img > 0.5).astype(np.int32)
    
    output_data = np.column_stack((x_coords, y_coords, binary_img.flatten()))
    out_txt_path = os.path.join(out_dir, f'comsol_topology_{i:03d}.txt')
    np.savetxt(out_txt_path, output_data, fmt=['%f', '%f', '%d'], header='x y value', comments='% ')
    
    # ==========================
    # 2. 提取并记录边界条件参数
    # conditions 通道对应: 0: 温度, 1: 入口位置, 2: 出口位置, 3: 体积分数
    # ==========================
    inlet_mask = conditions[i, 1] > 0.0
    outlet_mask = conditions[i, 2] > 0.0
    vf_val = conditions[i, 3].mean() # 取均值即为全局体积分数
    vf_val = (vf_val + 1.0) / 2.0    # 反归一化：将 [-1, 1] 映射回 [0, 1] 的物理真实体积分数
    
    def extract_w_c(mask):
        y, x = np.where(mask)
        if len(y) > 0:
            # 比较宽跨度还是高跨度，以确定它是横向还是纵向开口
            if x.max() - x.min() > y.max() - y.min():
                return x.max() - x.min() + 1, x.mean()
            else:
                return y.max() - y.min() + 1, y.mean()
        return 0, 0
        
    w1, c1 = extract_w_c(inlet_mask)
    w2, c2 = extract_w_c(outlet_mask)
    
    summary_lines.append(f"{i:03d}\t{w1}\t{c1:.1f}\t{w2}\t{c2:.1f}\t{vf_val:.4f}")

# 将提取到的条件参数汇总写入 TXT
summary_path = os.path.join(out_dir, 'boundary_conditions_summary.txt')
with open(summary_path, 'w', encoding='utf-8') as f:
    f.write('\n'.join(summary_lines))

print(f"Successfully decoded 100 samples to {out_dir}")
print(f"Summary saved to {summary_path}")
