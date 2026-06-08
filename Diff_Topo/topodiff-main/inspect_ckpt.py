import torch
import sys
ckpt = torch.load("checkpoints/diff_logdir_train1564/ema_0.9999_000000.pt", map_location="cpu")
for k, v in ckpt.items():
    if "weight" in k and len(v.shape) == 4:
        print(k, v.shape)
