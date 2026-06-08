import math
import random

from PIL import Image
import blobfile as bf
import numpy as np
from torch.utils.data import DataLoader, Dataset


def load_data(
    *,
    data_dir,
    batch_size,
    image_size,
    deterministic=False,
):
    """
    Create the generator used for training the main diffusion model.
    The dataset should contain:
    - the .npy files of merged flow channel data (6, 64, 64).

    :param data_dir: the dataset directory.
    :param batch_size: the batch size of each returned pair.
    :param image_size: the size of the images (ignored for our direct npy loading).
    :param deterministic: if True, yield results in a deterministic order.
    """
   
    if not data_dir:
        raise ValueError("unspecified data directory")
    all_files = _list_npy_files_recursively(data_dir)
    
    dataset = FlowChannelDataset(
        all_files,
        shard=0,
        num_shards=1
    )
    if deterministic:
        loader = DataLoader(
            dataset, batch_size=batch_size, shuffle=False, num_workers=0, drop_last=True
        )
    else:
        loader = DataLoader(
            dataset, batch_size=batch_size, shuffle=True, num_workers=0, drop_last=True
        )
    while True:
        yield from loader


def _list_npy_files_recursively(data_dir):
    npy_files = []
    for entry in sorted(bf.listdir(data_dir)):
        full_path = bf.join(data_dir, entry)
        ext = entry.split(".")[-1]
        if "." in entry and ext.lower() == "npy":
            npy_files.append(full_path)
        elif bf.isdir(full_path):
            npy_files.extend(_list_npy_files_recursively(full_path))
    return npy_files


class FlowChannelDataset(Dataset):
    def __init__(
        self,
        npy_paths,
        shard=0,
        num_shards=1
    ):
        super().__init__()
        self.local_files = npy_paths[shard:][::num_shards]

    def __len__(self):
        return len(self.local_files)

    def __getitem__(self, idx):
        file_path = self.local_files[idx]
        # Data shape is originally (6, 64, 64) as saved by build_dataset.py
        data = np.load(file_path).astype(np.float32)
        
        # Channel 0: structure (values are presumably 0 / 1)
        # We need to normalize structure from [0, 1] to [-1, 1] for diffusion
        structure = data[0:1, :, :]
        # Since topology binarization usually has 0 and 1, we map:
        # 0 -> -1
        # 1 ->  1
        # (val * 2) - 1
        structure = structure * 2.0 - 1.0
        
        # Channel 1~4: conditions
        conditions = data[1:5, :, :]
        
        # 1. Normalize Temperature field (index 0 of conditions) to [0, 1]
        t_min = conditions[0].min()
        t_max = conditions[0].max()
        if t_max > t_min:
            conditions[0] = (conditions[0] - t_min) / (t_max - t_min)
        else:
            conditions[0] = 0.0
            
        # 2. Map all 5 constraint channels from roughly [0, 1] to [-1, 1]
        conditions = conditions * 2.0 - 1.0
        
        out_dict = {}
        return structure, conditions, out_dict