import os
import glob
import numpy as np

def main():
    # 路径配置
    sim_dir = r"D:\Diff_Topo\开源\topodiff-Evaluate\1564dataset\3批量导入comsol再仿真\再仿真温度场"
    gt_dir = r"D:\Diff_Topo\开源\topodiff-main\data\flow_channels_1764_test"

    # 获取所有生成的 txt 文件
    sim_files = sorted(glob.glob(os.path.join(sim_dir, "*.txt")))
    gt_files = sorted(glob.glob(os.path.join(gt_dir, "*.npy")))

    print(f"Found {len(sim_files)} COMSOL simulated temperature files.")
    print(f"Found {len(gt_files)} Ground Truth files in test set.")

    total_mae = 0.0
    total_mre = 0.0
    valid_count = 0

    for sim_file in sim_files:
        # 文件名形如：模型结构导入comsol仿真温度场_000.txt
        basename = os.path.basename(sim_file)
        # 提取索引 (000, 001, etc.)
        idx_str = basename.replace("模型结构导入comsol仿真温度场_", "").replace(".txt", "")
        try:
            idx = int(idx_str)
        except ValueError:
            print(f"Skipping {basename} (Cannot parse index)")
            continue
        
        if idx >= len(gt_files):
            print(f"Index {idx} out of range for GT files!")
            continue

        # 读取 COMSOL 仿真温度场
        sim_data = np.loadtxt(sim_file, comments='%')
        sim_T = sim_data[:, 2].reshape(64, 64).astype(np.float32)

        # 读取真实拓扑优化的温度场 (channel 1 是温度场)
        gt_data = np.load(gt_files[idx])
        gt_T = gt_data[1].astype(np.float32)

        # 计算误差
        mae = np.mean(np.abs(sim_T - gt_T))
        # 避免除以 0（实际上开尔文温度不可能为0，但保守起见加个极小值）
        mre = np.mean(np.abs(sim_T - gt_T) / (gt_T + 1e-6))

        total_mae += mae
        total_mre += mre
        valid_count += 1

    if valid_count > 0:
        mean_mae = total_mae / valid_count
        mean_mre = total_mre / valid_count
        print("\n" + "="*40)
        print("          EVALUATION RESULTS          ")
        print("="*40)
        print(f"Valid paired samples evaluated : {valid_count}")
        print(f"Mean Absolute Error (MAE)      : {mean_mae:.4f} K")
        print(f"Mean Relative Error (MRE)      : {mean_mre:.6f} ({mean_mre*100:.4f}%)")
        print("="*40)
    else:
        print("No valid paired samples found to evaluate.")

if __name__ == "__main__":
    main()
