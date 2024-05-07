#!/usr/bin/env python

import os
import sys
import numpy as np
import cv2

def FFT(img):

    f = np.fft.fft2(img)
    fshift = np.fft.fftshift(f)
    spec = 20*np.log(np.abs(fshift))

    return spec

def usage():
    print(f"Usage: {sys.argv[0]} <image>")

if __name__ == "__main__":

    if len(sys.argv) < 2:
        usage()
        exit()        

    for filename in sys.argv[1:]:
        print(filename)

        basename = os.path.basename(filename)
        parts = basename.split('.')
        name = parts[0]

        image = cv2.imread(filename, cv2.IMREAD_GRAYSCALE)
        spec = FFT(image)

        cv2.imwrite(f'{name}.FFT.png', spec)
