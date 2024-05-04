#! /usr/bin/env python
import math
import sys
import sys,os
import iio as piio
import numpy as np
import argparse
from scipy.ndimage.morphology import binary_dilation

# Create visual PNG visual detections for the IPOL demo.
# Please use the visu.py script in Utilities for better
# visualization otherwise

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('pixel', type=str, help=('path to input image'))
    parser.add_argument('radius', type=str, help=('path to output image'))
    parser.add_argument('nfa', type=float, help=('path to output image'))
    parser.add_argument('output', type=str, help=('path to output image'))
    args = parser.parse_args()

    # Create a version of the input which has been toned down
    pixelNFA = piio.read(args.pixel).astype(np.float)
    print(pixelNFA.shape)
    radiusNFA = piio.read(args.radius).astype(np.float)
    print(pixelNFA.shape)

    # For speed we show detections with the same disk of size 2 instead of 2 or 4
    detections = (pixelNFA < args.nfa) * (radiusNFA == 2)
    detections = np.squeeze(detections)
    ball = np.zeros((5, 5))
    ball[2, 1] = 1
    ball[1, 2] = 1
    ball[2, 2] = 1
    ball[2, 3] = 1
    ball[3, 2] = 1

    ball[1, 1] = 1
    ball[1, 3] = 1
    ball[3, 1] = 1
    ball[3, 3] = 1
    ball[0, 2] = 1
    ball[2, 0] = 1
    ball[4, 2] = 1
    ball[2, 4] = 1
    detection = binary_dilation(detections, ball)

    detections = (pixelNFA < args.nfa) * (radiusNFA == 4)
    detections = np.squeeze(detections)
    ball = np.zeros((7, 7))
    ball[3, 2] = 1
    ball[2, 3] = 1
    ball[3, 3] = 1
    ball[3, 4] = 1
    ball[4, 3] = 1

    ball[2, 2] = 1
    ball[2, 4] = 1
    ball[4, 2] = 1
    ball[4, 4] = 1
    ball[1, 3] = 1
    ball[3, 1] = 1
    ball[5, 3] = 1
    ball[3, 5] = 1

    ball[1, 2] = 1
    ball[2, 1] = 1
    ball[1, 4] = 1
    ball[4, 1] = 1
    ball[5, 2] = 1
    ball[2, 5] = 1
    ball[5, 4] = 1
    ball[4, 5] = 1
    ball[0, 3] = 1
    ball[3, 0] = 1
    ball[6, 3] = 1
    ball[3, 6] = 1
    detection += binary_dilation(detections, ball)

    # Create color detections
    color_detection = np.zeros((pixelNFA.shape[0], pixelNFA.shape[1], 3))
    color_detection[:,:,1] = (detection == 0)*255.
    color_detection[:,:,0] = (detection > 0)*255.
    piio.write(args.output, color_detection)

