#!/usr/bin/env python

import sys
import numpy as np
import cv2
import matplotlib.pyplot as plt

from SIFT import SIFT

def usage():
    print("Usage: ./main.py <image>")

if __name__ == "__main__":

    if len(sys.argv) != 2:
        usage()
        exit()

    image = cv2.imread(sys.argv[1])

    sift_image = SIFT(image)
    #orb_image = IRB(image)
    #freak_image = FREAK(image)

    plt.imshow(sift_image)
    plt.show()
    
