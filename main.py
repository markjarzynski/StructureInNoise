#!/usr/bin/env python

import sys
import os
import numpy as np
import cv2
import matplotlib.pyplot as plt

from ORB import ORBClass
from SIFT import SIFTClass
from BRISK import BRISKClass
from FREAK import FREAKClass
from FeatureDetector import FeatureDetector

def usage():
    print(f"Usage: {sys.argv[0]} <image>")

if __name__ == "__main__":

    if len(sys.argv) != 2:
        usage()
        exit()

    filename = sys.argv[1]
    image = cv2.imread(filename)

    if image is None:
        print("Error: Image did not load.")
        exit()

    basename = os.path.basename(filename)
    parts = basename.split('.')
    name = parts[0]

    sift = SIFTClass(image)
    sift.run()
    sift.printFirst()
    sift.writeFirstImage(f'{name}.SIFT.png')

    orb = ORBClass(image)
    orb.run()
    orb.printFirst()
    orb.writeFirstImage(f'{name}.ORB.png')

    brisk = BRISKClass(image)
    brisk.run()
    brisk.printFirst()
    brisk.writeFirstImage(f'{name}.BRISK.png')

    freak = FREAKClass(image)
    freak.run()
    freak.printFirst()
    freak.writeFirstImage(f'{name}.FREAK.png')
