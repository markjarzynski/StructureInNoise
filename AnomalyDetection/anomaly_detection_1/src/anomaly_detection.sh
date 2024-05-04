#!/bin/bash
#/*
# * Copyright (c) 2017, Thibaud Ehret <ehret.thibaud@gmail.com>
# * All rights reserved.
# *
# * This program is free software: you can use, modify and/or
# * redistribute it under the terms of the GNU General Public
# * License as published by the Free Software Foundation, either
# * version 3 of the License, or (at your option) any later
# * version. You should have received a copy of this license along
# * this program. If not, see <http://www.gnu.org/licenses/>.
# */


input=$1
reference=$input
usenn=False
pathnn=''
px=8
np=16
lognfa=-2

if [ -n "$2" ]; then
    reference=$2
fi
if [ -n "$3" ]; then
   lognfa=$3
fi
if [ -n "$4" ]; then
   sp=$4
fi
if [ -n "$5" ]; then
   np=$5
fi
if [ -n "$6" ]; then
    usenn=True
    pathnn=$6
    layer=$7
fi

python decompose.py $input $usenn scale_ 4 _0.tiff
python decompose.py $reference $usenn scale_ 4 _1.tiff


if [ "$usenn" = True ]; then
    for scale in 0 1 2 3
    do
        ./output_from_network.py $layer scale_${scale}_0.tiff ./ $pathnn
        mv nn_output.tiff scale_${scale}_0.tiff    
		./project_pca_upper.py scale_${scale}_0.tiff ./
        ./output_from_network.py $layer scale_${scale}_1.tiff ./ $pathnn
        mv nn_output.tiff scale_${scale}_1.tiff    
		./project_pca_upper.py scale_${scale}_1.tiff ./
    done
fi

for scale in 0 1 2 3
do
    ./main_model -i scale_${scale}_0.tiff -og $input -ref scale_${scale}_1.tiff -model model_$scale.tiff -res residual_$scale.tiff -px $px -np $np
    ./convert.py residual_$scale.tiff residual_${scale}_g.tiff
    ./main_detection -og $input -res residual_${scale}_g.tiff -pxlnfa pixelNFA_$scale.tiff -rdnfa radiusNFA_$scale.tiff
done

python visu.py $input 0 4 ./ pixelNFA_ radiusNFA_ detections.pdf $lognfa

rm pixelNFA_* 
rm radiusNFA_*
rm scale_*
rm model_*
rm residual_*
rm input_0_rescaled.png
