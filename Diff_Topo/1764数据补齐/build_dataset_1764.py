import os
import numpy as np
from tqdm import tqdm

base_dir = r"D:\Diff_Topo\开源\1764数据补齐"
out_dir = r"D:\Diff_Topo\开源\topodiff-main\data\flow_channels_1764"

os.makedirs(out_dir, exist_ok=True)

# 按照之前的 6 通道顺序
dirs = [
    "02_out拓扑结构_二值化",
    "10_out仿真温度场",
    "06_out入口位置",
    "07_out出口位置",
    "08_out体积分数",
    "09_out热源分布"
]

def load_txt(file_path):
    data = np.loadtxt(file_path, comments='%', encoding='utf-8')
    return data[:, 2].reshape(64, 64).astype(np.float32)

# Get sorted list of files for each directory
files_per_dir = []
for d in dirs:
    path = os.path.join(base_dir, d)
    files = sorted([f for f in os.listdir(path) if f.endswith('.txt')])
    files_per_dir.append(files)

# Safety check
num_samples = len(files_per_dir[0])
for i in range(1, len(dirs)):
    assert len(files_per_dir[i]) == num_samples, f"Mismatch in number of files in {dirs[i]}"

print(f"Found {num_samples} samples per directory. Starting processing...")

for i in tqdm(range(num_samples)):
    channels = []
    for d_idx, d in enumerate(dirs):
        file_path = os.path.join(base_dir, d, files_per_dir[d_idx][i])
        channel_data = load_txt(file_path)
        channels.append(channel_data)
    
    # Stack into shape (6, 64, 64)
    sample = np.stack(channels, axis=0) 
    
    out_file = os.path.join(out_dir, f"data_{i:04d}.npy")
    np.save(out_file, sample)

print(f"Data processing complete! Saved to {out_dir}")
