import os
import numpy as np
import pandas as pd
import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import Dataset, DataLoader
from sklearn.metrics import r2_score
from scipy.stats import spearmanr
import matplotlib.pyplot as plt
import seaborn as sns

# -----------------
# 1. Dataset
# -----------------
class TopoTempDataset(Dataset):
    def __init__(self, csv_file, split="train", t_mean=None, t_std=None):
        self.df = pd.read_csv(csv_file)
        self.df = self.df[self.df["split"] == split].reset_index(drop=True)
        
        # 为了支持标签的标准化
        self.t_mean = t_mean
        self.t_std = t_std

    def __len__(self):
        return len(self.df)

    def __getitem__(self, idx):
        row = self.df.iloc[idx]
        img_path = row["image_path"]
        t_avg = row["T_avg"]
        
        # 从 npy 中读取结构通道，[0] 是结构 (通常为 0 和 1)
        data = np.load(img_path)
        struct = data[0].astype(np.float32)
        
        # 确保归一化在 [0, 1] 之间
        if struct.min() < 0:
            struct = (struct + 1.0) / 2.0  # 假设它是在 [-1, 1] 之间
            
        img_tensor = torch.from_numpy(struct).unsqueeze(0) # [1, 64, 64]
        
        # 标准化标签
        if self.t_mean is not None and self.t_std is not None:
            t_label = (t_avg - self.t_mean) / self.t_std
        else:
            t_label = t_avg
            
        return img_tensor, torch.tensor(t_label, dtype=torch.float32), torch.tensor(t_avg, dtype=torch.float32)


# -----------------
# 2. Model
# -----------------
class CNNRegressor(nn.Module):
    def __init__(self):
        super(CNNRegressor, self).__init__()
        self.encoder = nn.Sequential(
            nn.Conv2d(1, 16, kernel_size=3, padding=1),
            nn.BatchNorm2d(16),
            nn.ReLU(),
            nn.MaxPool2d(2), # 32x32
            
            nn.Conv2d(16, 32, kernel_size=3, padding=1),
            nn.BatchNorm2d(32),
            nn.ReLU(),
            nn.MaxPool2d(2), # 16x16
            
            nn.Conv2d(32, 64, kernel_size=3, padding=1),
            nn.BatchNorm2d(64),
            nn.ReLU(),
            nn.MaxPool2d(2), # 8x8
            
            nn.Conv2d(64, 128, kernel_size=3, padding=1),
            nn.BatchNorm2d(128),
            nn.ReLU(),
            
            nn.AdaptiveAvgPool2d(1) # [B, 128, 1, 1]
        )
        self.regressor = nn.Sequential(
            nn.Flatten(),
            nn.Linear(128, 64),
            nn.ReLU(),
            nn.Dropout(0.2),
            nn.Linear(64, 1)
        )

    def forward(self, x):
        feat = self.encoder(x)
        out = self.regressor(feat)
        return out.squeeze(1)


# -----------------
# 3. Main Training
# -----------------
def main():
    base_dir = r"D:\Diff_Topo\开源\topodiff-Guide"
    csv_path = os.path.join(base_dir, "data", "temperature_regression_dataset.csv")
    
    # 获取训练集的 T_avg 均值和标准差
    df = pd.read_csv(csv_path)
    train_df = df[df["split"] == "train"]
    t_mean = train_df["T_avg"].mean()
    t_std = train_df["T_avg"].std()
    
    print(f"Dataset stats -> T_mean: {t_mean:.2f}, T_std: {t_std:.2f}")

    train_dataset = TopoTempDataset(csv_path, split="train", t_mean=t_mean, t_std=t_std)
    test_dataset = TopoTempDataset(csv_path, split="test", t_mean=t_mean, t_std=t_std)
    
    train_loader = DataLoader(train_dataset, batch_size=32, shuffle=True, num_workers=0)
    test_loader = DataLoader(test_dataset, batch_size=32, shuffle=False, num_workers=0)
    
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model = CNNRegressor().to(device)
    
    criterion = nn.HuberLoss()
    optimizer = optim.AdamW(model.parameters(), lr=1e-3, weight_decay=1e-4)
    scheduler = optim.lr_scheduler.ReduceLROnPlateau(optimizer, mode='min', factor=0.5, patience=10)
    
    epochs = 200
    best_mae = float('inf')
    best_model_path = os.path.join(base_dir, "checkpoints", "best_temp_cnn_regressor.pth")
    
    for epoch in range(epochs):
        # 训练阶段
        model.train()
        train_loss = 0.0
        for imgs, labels, _ in train_loader:
            imgs, labels = imgs.to(device), labels.to(device)
            optimizer.zero_grad()
            preds = model(imgs)
            loss = criterion(preds, labels)
            loss.backward()
            optimizer.step()
            train_loss += loss.item() * imgs.size(0)
            
        train_loss /= len(train_dataset)
        
        # 评估阶段
        model.eval()
        test_loss = 0.0
        all_preds = []
        all_trues = []
        
        with torch.no_grad():
            for imgs, labels, true_t in test_loader:
                imgs, labels = imgs.to(device), labels.to(device)
                preds = model(imgs)
                loss = criterion(preds, labels)
                test_loss += loss.item() * imgs.size(0)
                
                # 反标准化得到真实的温度预测值
                preds_real = preds.cpu().numpy() * t_std + t_mean
                trues_real = true_t.cpu().numpy()
                
                all_preds.extend(preds_real)
                all_trues.extend(trues_real)
                
        test_loss /= len(test_dataset)
        
        # 计算各种指标
        all_preds = np.array(all_preds)
        all_trues = np.array(all_trues)
        
        mae = np.mean(np.abs(all_preds - all_trues))
        rmse = np.sqrt(np.mean((all_preds - all_trues)**2))
        r2 = r2_score(all_trues, all_preds)
        spearman_corr, _ = spearmanr(all_trues, all_preds)
        
        scheduler.step(mae)
        
        if mae < best_mae:
            best_mae = mae
            torch.save(model.state_dict(), best_model_path)
            
        if (epoch + 1) % 10 == 0 or epoch == 0:
            print(f"Epoch [{epoch+1:3d}/{epochs}] "
                  f"Train Loss: {train_loss:.4f} | Test Loss: {test_loss:.4f} | "
                  f"MAE: {mae:.2f}K | RMSE: {rmse:.2f}K | R2: {r2:.4f} | SpC: {spearman_corr:.4f}")

    print(f"Training completed. Best Test MAE: {best_mae:.2f}K")
    
    # -----------------
    # 4. Final Evaluation & Plots
    # -----------------
    print("Running final evaluation on best model...")
    model.load_state_dict(torch.load(best_model_path))
    model.eval()
    
    final_preds = []
    final_trues = []
    ids = test_dataset.df["id"].values
    
    with torch.no_grad():
        for imgs, _, true_t in test_loader:
            imgs = imgs.to(device)
            preds = model(imgs).cpu().numpy()
            preds_real = preds * t_std + t_mean
            final_preds.extend(preds_real)
            final_trues.extend(true_t.numpy())
            
    final_preds = np.array(final_preds)
    final_trues = np.array(final_trues)
    
    abs_errors = np.abs(final_preds - final_trues)
    rel_errors = abs_errors / final_trues
    
    # 生成预测 CSV
    results_df = pd.DataFrame({
        "id": ids,
        "T_true": final_trues,
        "T_pred": final_preds,
        "abs_error": abs_errors,
        "rel_error": rel_errors
    })
    results_csv = os.path.join(base_dir, "results", "temp_regression_predictions.csv")
    results_df.to_csv(results_csv, index=False)
    print(f"Predictions saved to {results_csv}")
    
    # 画图：真实值 vs 预测值
    plt.figure(figsize=(6, 6))
    plt.scatter(final_trues, final_preds, alpha=0.6, color='blue')
    
    # 画对角线
    min_val = min(final_trues.min(), final_preds.min())
    max_val = max(final_trues.max(), final_preds.max())
    plt.plot([min_val, max_val], [min_val, max_val], 'r--')
    
    plt.xlabel('True T_avg (K)')
    plt.ylabel('Predicted T_avg (K)')
    plt.title('True vs Predicted Average Temperature')
    plt.grid(True, linestyle=':', alpha=0.7)
    plt.savefig(os.path.join(base_dir, "results", "pred_vs_true.png"), dpi=150, bbox_inches='tight')
    plt.close()
    
    # 画图：误差分布直方图
    plt.figure(figsize=(6, 4))
    sns.histplot(abs_errors, bins=30, kde=True, color='purple')
    plt.xlabel('Absolute Error (K)')
    plt.ylabel('Frequency')
    plt.title('Distribution of Absolute Errors')
    plt.savefig(os.path.join(base_dir, "results", "error_distribution.png"), dpi=150, bbox_inches='tight')
    plt.close()

if __name__ == "__main__":
    main()
