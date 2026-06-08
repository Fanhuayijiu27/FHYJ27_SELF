import os
import numpy as np
import random
from tqdm import tqdm
import shutil

base_dir = r"D:\Diff_Topo\开源\1764数据补齐"
out_train_dir = r"D:\Diff_Topo\开源\topodiff-main\data\flow_channels_1764_train"
out_test_dir = r"D:\Diff_Topo\开源\topodiff-main\data\flow_channels_1764_test"

# 清理并创建目录
if os.path.exists(out_train_dir):
    shutil.rmtree(out_train_dir)
if os.path.exists(out_test_dir):
    shutil.rmtree(out_test_dir)
    
os.makedirs(out_train_dir, exist_ok=True)
os.makedirs(out_test_dir, exist_ok=True)

# 仅保留 5 通道，去掉了废弃的热源分布
dirs = [
    "02_out拓扑结构_二值化",
    "10_out仿真温度场",
    "06_out入口位置",
    "07_out出口位置",
    "08_out体积分数"
]

def load_txt(file_path):
    data = np.loadtxt(file_path, comments='%', encoding='utf-8')
    return data[:, 2].reshape(64, 64).astype(np.float32)

files_per_dir = []
for d in dirs:
    path = os.path.join(base_dir, d)
    files = sorted([f for f in os.listdir(path) if f.endswith('.txt')])
    files_per_dir.append(files)

num_samples = len(files_per_dir[0])
for i in range(1, len(dirs)):
    assert len(files_per_dir[i]) == num_samples, f"Mismatch in number of files in {dirs[i]}"

print(f"Found {num_samples} samples per directory. Preparing 5-channel data...")

# 随机打乱索引以分割数据集
indices = list(range(num_samples))
random.seed(42) # 固定随机种子以保证可复现
random.shuffle(indices)

train_indices = indices[:1564]
test_indices = indices[1564:]

print(f"Splitting: {len(train_indices)} for train, {len(test_indices)} for test.")

def process_and_save(idx_list, out_folder, prefix=""):
    for idx in tqdm(idx_list, desc=f"Processing {prefix}"):
        channels = []
        for d_idx, d in enumerate(dirs):
            file_path = os.path.join(base_dir, d, files_per_dir[d_idx][idx])
            channel_data = load_txt(file_path)
            channels.append(channel_data)
        
        # Stack into shape (5, 64, 64)
        sample = np.stack(channels, axis=0) 
        
        out_file = os.path.join(out_folder, f"data_{idx:04d}.npy")
        np.save(out_file, sample)

process_and_save(train_indices, out_train_dir, "Train Set")
process_and_save(test_indices, out_test_dir, "Test Set")

print("Dataset successfully split and packaged into 5 channels!")
