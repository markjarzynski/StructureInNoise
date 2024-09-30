#!/usr/bin/bash

source venv/bin/activate

outdir=results/group/
mkdir -p $outdir

for image in rng2img/murmur3.png rng2img/pcg3d.png rng2img/pcg4d.png rng2img/ranlim32_linear.png rng2img/ranlim32_nested.png rng2img/superfast.png rng2img/tea2.png rng2img/tea3.png rng2img/tea4.png rng2img/tea5.png rng2img/wang_linear.png rng2img/wang_nested.png rng2img/xorshift32_linear.png rng2img/xorshift32_nested.png rng2img/xxhash32.png ;
do
    base=$(basename $image .png)
    ./group-SIFT.py $image > $outdir$base.SIFT.txt
    ./group-ORB.py $image > $outdir$base.ORB.txt
    ./group-FREAK.py $image > $outdir$base.FREAK.txt
    ./group-BRISK.py $image > $outdir$base.BRISK.txt
done
