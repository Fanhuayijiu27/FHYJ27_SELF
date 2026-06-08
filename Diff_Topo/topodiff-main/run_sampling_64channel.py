import os
import subprocess
import sys

env = os.environ.copy()
env["PYTHONPATH"] = "."
env["TOPODIFF_LOGDIR"] = "Generate_results/eval_1764_test_64channel_035000"

os.makedirs(env["TOPODIFF_LOGDIR"], exist_ok=True)

cmd = [
    r"D:\AnacondaAPP\envs\1_jupyter_model_train\python.exe",
    "scripts/flow_channel_sample.py",
    "--model_path", "checkpoints/diff_logdir_train1564(64channel)/ema_0.9999_035000.pt",
    "--data_dir", "data/flow_channels_1764_test",
    "--num_samples", "200",
    "--batch_size", "10",
    "--image_size", "64",
    "--num_channels", "64",
    "--num_res_blocks", "2",
    "--attention_resolutions", "16,8",
    "--use_fp16", "True"
]

print("Running command:", " ".join(cmd))
subprocess.run(cmd, env=env, check=True)
