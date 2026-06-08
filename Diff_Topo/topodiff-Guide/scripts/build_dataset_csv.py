import os
import glob
import numpy as np
import pandas as pd
from tqdm import tqdm

def main():
    # 建立目录结构
    base_dir = r"D:\Diff_Topo\开源\topodiff-Guide"
    os.makedirs(os.path.join(base_dir, "data"), exist_ok=True)
    os.makedirs(os.path.join(base_dir, "scripts"), exist_ok=True)
    os.makedirs(os.path.join(base_dir, "checkpoints"), exist_ok=True)
    os.makedirs(os.path.join(base_dir, "results"), exist_ok=True)
    
    # 获取已经切分好的 npy 数据集目录
    train_dir = r"D:\Diff_Topo\开源\topodiff-main\data\flow_channels_1764_train"
    test_dir = r"D:\Diff_Topo\开源\topodiff-main\data\flow_channels_1764_test"
    
    records = []
    
    def process_dir(directory, split_name):
        files = sorted(glob.glob(os.path.join(directory, "*.npy")))
        for fpath in tqdm(files, desc=f"Processing {split_name}"):
            basename = os.path.basename(fpath)
            sample_id = basename.replace("data_", "").replace(".npy", "")
            
            # 读取 npy 数据
            # npy 的 shape 是 (5, 64, 64)
            # channel 0: 结构 (0或1)
            # channel 1: 温度场 (真实物理值 K)
            data = np.load(fpath)
            t_field = data[1]
            
            # 计算平均温度
            t_avg = np.mean(t_field)
            
            records.append({
                "id": sample_id,
                # 因为原始数据打包在同一个 npy 里面，我们就把 image_path 和 temp_field_path 都指向它
                "image_path": fpath,
                "temp_field_path": fpath,
                "T_avg": float(t_avg),
                "split": split_name
            })

    process_dir(train_dir, "train")
    process_dir(test_dir, "test")
    
    # 生成 CSV
    df = pd.DataFrame(records)
    csv_path = os.path.join(base_dir, "data", "temperature_regression_dataset.csv")
    df.to_csv(csv_path, index=False)
    print(f"\nSuccessfully generated dataset CSV at {csv_path}")
    print(df["split"].value_counts())

if __name__ == "__main__":
    main()
