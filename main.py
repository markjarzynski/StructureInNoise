#!/usr/bin/env python

import sys
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

    image = cv2.imread(sys.argv[1])

    if image is None:
        print("Error: Image did not load.")
        exit()

    sift = SIFTClass(image)
    sift.run()
    sift.printFirst()

    orb = ORBClass(image)
    orb.run()
    orb.printFirst()

    brisk = BRISKClass(image)
    brisk.run()
    brisk.printFirst()

    freak = FREAKClass(image)
    freak.run()
    freak.printFirst()
