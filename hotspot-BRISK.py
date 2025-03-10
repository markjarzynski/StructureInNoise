#!/usr/bin/env python

import sys
import os
import numpy as np
import cv2

from ORB import ORBClass
from SIFT import SIFTClass
from BRISK import BRISKClass
from FREAK import FREAKClass
from FeatureDetector import FeatureDetector

def usage():
    print(f"Usage: {sys.argv[0]} <image>")

if __name__ == "__main__":

    if len(sys.argv) < 2:
        usage()
        exit()

    print('HASH,BRISK')

    for filename in sys.argv[1:]:

        basename = os.path.basename(filename)
        parts = basename.split('.')
        name = parts[0]

        image = cv2.imread(filename)

        if image is None:
            print("Error: Image did not load.")
            continue

        basename = os.path.basename(filename)
        parts = basename.split('.')
        name = parts[0]

        brisk = BRISKClass(image, name)
        brisk.extractFeatures()
        brisk.computeGroup()
        brisk.writeHotspotImage()
        brisk.gradeHotspot()
