"""
Sample directly from the trained TopoDiff model using our 5-channel flow data (1 structure + 4 conditions).
"""

import argparse
import os
import numpy as np
import torch as th

from topodiff import dist_util, logger
from topodiff.script_util import (
    model_and_diffusion_defaults,
    create_model_and_diffusion,
    add_dict_to_argparser,
    args_to_dict,
)
from topodiff.image_datasets_diffusion_model import load_data

def main():
    args = create_argparser().parse_args()

    # Even though we disabled DDP, we still run this to set env vars
    dist_util.setup_dist()
    logger.configure(dir=os.getenv("TOPODIFF_LOGDIR"))

    logger.log("creating model and diffusion...")
    model, diffusion = create_model_and_diffusion(
        **args_to_dict(args, model_and_diffusion_defaults().keys())
    )
    
    logger.log(f"Loading checkpoint: {args.model_path}")
    model.load_state_dict(
        dist_util.load_state_dict(args.model_path, map_location="cpu")
    )
    model.to(dist_util.dev())
    if args.use_fp16:
        model.convert_to_fp16()
    model.eval()

    logger.log("loading data conditions...")
    # Load our custom 5-channel dataset (structure, conditions, _)
    data = load_data(
        data_dir=args.data_dir,
        batch_size=args.batch_size,
        image_size=args.image_size,
        deterministic=True # Important for evaluating in order
    )

    logger.log("sampling...")
    all_images = []
    all_conditions = []
    
    while len(all_images) * args.batch_size < args.num_samples:
        model_kwargs = {}
        sample_fn = (
            diffusion.p_sample_loop if not args.use_ddim else diffusion.ddim_sample_loop
        )
        # Our dataset yields: structure (1,64,64) and conditions (4,64,64)
        _, conditions_batch, _ = next(data)
        conditions_batch = conditions_batch.to(dist_util.dev())

        # TopoDiff's GaussianDiffusion expects cons, loads, BCs as separate arguments in p_sample_loop.
        # It internally concatenates cons and loads as: th.cat([cons, loads], dim=1)
        # So we pass our 4-channel conditions as 'cons', and empty tensors for loads and BCs.
        empty_loads = th.empty((args.batch_size, 0, args.image_size, args.image_size), device=dist_util.dev())
        empty_BCs = th.empty((args.batch_size, 0, args.image_size, args.image_size), device=dist_util.dev())

        sample = sample_fn(
            model,
            (args.batch_size, 1, args.image_size, args.image_size),
            conditions_batch,
            empty_loads,
            empty_BCs,
            clip_denoised=args.clip_denoised,
            model_kwargs=model_kwargs,
            cond_fn_1=None,  # No regressor
            cond_fn_2=None,  # No fm classifier
            device=dist_util.dev(),
        )
        # Convert [-1, 1] back to [0, 1]
        sample = (sample + 1) / 2.0
        sample = sample.clamp(0, 1)

        all_images.append(sample.cpu().numpy())
        all_conditions.append(conditions_batch.cpu().numpy())
        logger.log(f"created {min((len(all_images)) * args.batch_size, args.num_samples)} / {args.num_samples} samples")

    arr = np.concatenate(all_images, axis=0)[: args.num_samples]
    cond_arr = np.concatenate(all_conditions, axis=0)[: args.num_samples]

    out_path = os.path.join(logger.get_dir(), f"generated_flow_channels.npz")
    logger.log(f"saving to {out_path}")
    np.savez(out_path, images=arr, conditions=cond_arr)

    logger.log("sampling complete")

def create_argparser():
    defaults = dict(
        data_dir="",
        clip_denoised=True,
        num_samples=10,
        batch_size=1,
        use_ddim=False,
        model_path="",
        use_fp16=True,
    )
    defaults.update(model_and_diffusion_defaults())
    parser = argparse.ArgumentParser()
    add_dict_to_argparser(parser, defaults)
    return parser

if __name__ == "__main__":
    main()
