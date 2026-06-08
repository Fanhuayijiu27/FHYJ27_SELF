import numpy as np
import glob
import os

def main():
    npz_path = r'D:\Diff_Topo\开源\topodiff-main\Generate_results\eval_1764_test_64channel_030000\generated_flow_channels.npz'
    test_data_dir = r'D:\Diff_Topo\开源\topodiff-main\data\flow_channels_1764_test'
    
    print(f"Loading generated data from {npz_path}...")
    try:
        data = np.load(npz_path)
    except FileNotFoundError:
        print("Sampling is not yet finished. npz file not found.")
        return
        
    gen_images = data['images'] # shape [200, 1, 64, 64], values [-1, 1]
    conditions = data['conditions'] # shape [200, 4, 64, 64]
    
    # Binarize generated images ( > 0 means solid, < 0 means fluid/void)
    # Actually if range is [-1, 1], threshold is 0.
    gen_binary = (gen_images[:, 0, :, :] > 0.5).astype(np.float32)
    
    # Load all GT data from test set to match them
    print("Loading Ground Truth data to match...")
    gt_files = glob.glob(os.path.join(test_data_dir, "*.npy"))
    gt_structures = []
    gt_conditions = []
    
    for f in gt_files:
        d = np.load(f)
        # d shape is [5, 64, 64]
        gt_structures.append((d[0] * 2.0 - 1.0) > 0.0)
        gt_conditions.append(d[1:5])
        
    gt_structures = np.array(gt_structures, dtype=np.float32)
    gt_conditions = np.array(gt_conditions, dtype=np.float32)
    
    # Metrics accumulators
    ious = []
    vfes = []
    io_coverages = []
    rmses = []
    
    print("Evaluating metrics...")
    matched_indices = []
    
    for i in range(len(gen_images)):
        cond_i = conditions[i]
        
        # Find the matching GT
        # We can compute MSE between cond_i and all gt_conditions
        diffs = np.mean((gt_conditions - cond_i)**2, axis=(1, 2, 3))
        best_match_idx = np.argmin(diffs)
        matched_indices.append(best_match_idx)
        
        gt_struct = gt_structures[best_match_idx]
        gen_struct = gen_binary[i]
        
        # 1. IoU
        intersection = np.logical_and(gt_struct, gen_struct).sum()
        union = np.logical_or(gt_struct, gen_struct).sum()
        iou = intersection / union if union > 0 else 1.0
        ious.append(iou)
        
        # 2. RMSE
        rmse = np.sqrt(np.mean((gt_struct - gen_struct)**2))
        rmses.append(rmse)
        
        # 3. VFE
        # conditions[i, 3] is VF condition, normalized to [-1, 1].
        # However, GT structure's VF might slightly differ from the condition scalar.
        # Let's compare generated VF with the target condition VF.
        target_vf = (cond_i[3].mean() + 1.0) / 2.0
        actual_vf = gen_struct.mean()
        vfe = np.abs(actual_vf - target_vf)
        vfes.append(vfe)
        
        # 4. IO Coverage
        inlet_mask = cond_i[1] > 0.0
        outlet_mask = cond_i[2] > 0.0
        io_mask = np.logical_or(inlet_mask, outlet_mask)
        
        if io_mask.sum() > 0:
            coverage = gen_struct[io_mask].mean()
            io_coverages.append(coverage)
            
    print("\n========== EVALUATION RESULTS ==========")
    print(f"Number of samples evaluated: {len(gen_images)}")
    print(f"Mean IoU (平均交并比): {np.mean(ious):.4f}")
    print(f"Mean VFE (体积分数绝对误差): {np.mean(vfes):.4f}")
    print(f"Mean IO Coverage (入口与出口连通覆写率): {np.mean(io_coverages):.4f}")
    print(f"Mean RMSE (均方根误差): {np.mean(rmses):.4f}")
    print("========================================\n")

if __name__ == '__main__':
    main()
