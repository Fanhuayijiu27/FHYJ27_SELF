import numpy as np

npz_path = r'D:\Diff_Topo\开源\topodiff-main\Generate_results\eval_1764_test_070000\generated_flow_channels.npz'
data = np.load(npz_path)
images = data['images']  # shape [200, 1, 64, 64]
conditions = data['conditions']

print("Images shape:", images.shape)
print("Conditions shape:", conditions.shape)
print("Images range: min=", images.min(), "max=", images.max(), "mean=", images.mean())

# Check how many generated images are binary or close to constant
for i in range(5):
    img = images[i, 0]
    binary_img = img > 0.0
    print(f"Sample {i}: min={img.min():.4f}, max={img.max():.4f}, mean={img.mean():.4f}, active_fraction={binary_img.mean():.4f}")

# Let's check how many samples have active_fraction very close to 0 or 1
active_fractions = [(img[0] > 0.0).mean() for img in images]
print("Active fractions statistics:")
print("Min:", np.min(active_fractions))
print("Max:", np.max(active_fractions))
print("Mean:", np.mean(active_fractions))
print("Std:", np.std(active_fractions))
