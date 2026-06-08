import numpy as np
import os
import glob
import csv

# ==========================================
# 1. 路径配置
# ==========================================
# 原测试集真值温度场 (按字母顺序取前 100 个)
gt_dir = r"D:\Diff_Topo\开源\900数据补齐\out\04_out仿真温度场"
gt_files = sorted(glob.glob(os.path.join(gt_dir, '*.txt')))[:100]

# 我们刚刚生成的 COMSOL 再仿真温度场
sim_dir = r"D:\Diff_Topo\开源\topodiff-Evaluate\3批量导入comsol再仿真\再仿真温度场"

# 评估结果输出路径
out_csv = r"D:\Diff_Topo\开源\topodiff-Evaluate\4Evaluate\mean_temperature_evaluation.csv"
os.makedirs(os.path.dirname(out_csv), exist_ok=True)

# ==========================================
# 2. 读取数据并计算平均值与误差
# ==========================================
results = []
abs_errors = []
rel_errors = []

print("Starting evaluation...")
for i in range(100):
    sim_filename = f"模型结构导入comsol仿真温度场_{i:03d}.txt"
    sim_path = os.path.join(sim_dir, sim_filename)
    
    if not os.path.exists(sim_path):
        print(f"[Warning] Sim file missing: {sim_filename}")
        continue
        
    try:
        # 读取再仿真的温度数据 (第3列为数值)
        sim_data = np.loadtxt(sim_path, comments='%', encoding='utf-8')
        sim_temp = sim_data[:, 2]
        sim_mean = np.mean(sim_temp)
        
        # 读取原测试集的温度数据
        gt_data = np.loadtxt(gt_files[i], comments='%', encoding='utf-8')
        gt_temp = gt_data[:, 2]
        gt_mean = np.mean(gt_temp)
        
        # 计算误差
        abs_err = abs(sim_mean - gt_mean)
        rel_err = abs_err / gt_mean if gt_mean != 0 else 0.0
        
        abs_errors.append(abs_err)
        rel_errors.append(rel_err)
        
        results.append({
            'Sample_ID': f"{i:03d}",
            'Sim_Mean_Temp': round(sim_mean, 4),
            'GT_Mean_Temp': round(gt_mean, 4),
            'Abs_Error': round(abs_err, 4),
            'Rel_Error': round(rel_err, 6)
        })
    except Exception as e:
        print(f"[Error] Failed to process sample {i:03d}: {e}")

# ==========================================
# 3. 输出 CSV
# ==========================================
with open(out_csv, 'w', newline='', encoding='utf-8') as f:
    writer = csv.DictWriter(f, fieldnames=['Sample_ID', 'Sim_Mean_Temp', 'GT_Mean_Temp', 'Abs_Error', 'Rel_Error'])
    writer.writeheader()
    writer.writerows(results)

# ==========================================
# 4. 终端打印总结报告
# ==========================================
if len(results) > 0:
    mae = np.mean(abs_errors)
    mre = np.mean(rel_errors) * 100  # 转换为百分比
    
    print("\n" + "=" * 50)
    print("   温度场均值评估报告 (100组样本)   ")
    print("=" * 50)
    print(f"成功处理样本数: {len(results)} / 100")
    print("-" * 50)
    print(f"绝对误差均值 (MAE) : {mae:.4f} K")
    print(f"相对误差均值 (MRE) : {mre:.4f} %")
    print("=" * 50)
    print(f"详细的每个样本误差数据已保存至: \n{out_csv}")
else:
    print("\n[Error] 未成功处理任何样本，请检查路径。")
