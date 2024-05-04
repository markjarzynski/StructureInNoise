#! /usr/bin/env python
import math
import sys
import sys,os
import iio as piio
import numpy as np
import argparse

# Create PNG residuals for the IPOL demo.
# Please use a TIFF image viewer to look at residuals otherwise

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('input', type=str, help=('path to input image'))
    parser.add_argument('output', type=str, help=('path to output image'))
    args = parser.parse_args()

    # Create a version of the input which has been toned down
    img = piio.read(args.input).astype(np.float)
    for c in range(img.shape[2]):
        list_pixels = np.sort(img[:,:,c].flatten())
        min_pixel = list_pixels[int(1*list_pixels.shape[0]/100)]
        max_pixel = list_pixels[int(99*list_pixels.shape[0]/100)]
        print(min_pixel, max_pixel)

        img[:,:,c] = img[:,:,c] - min_pixel
        img[:,:,c] = img[:,:,c] / (max_pixel-min_pixel) * 255.

    img[img > 255.] = 255.
    img[img < 0.] = 0.
    piio.write(args.output, img)

