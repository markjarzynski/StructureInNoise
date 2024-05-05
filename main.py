#!/usr/bin/env python

import sys
import numpy as np
import cv2
import matplotlib.pyplot as plt

from ORB import ORB, ORBClass
from SIFT import SIFT, SIFTClass
from BRISK import BRISKClass
from FREAK import FREAKClass
from FeatureDetector import FeatureDetector

def usage():
    print("Usage: ./main.py <image>")

if __name__ == "__main__":

    if len(sys.argv) != 2:
        usage()
        exit()

    image = cv2.imread(sys.argv[1])

    freak = FREAKClass(image)
    freak.runTest()

    sift = SIFTClass(image)
    sift.runTest()

    orb_image = ORBClass(image)
    orb.runTest()

    brisk = BRISKClass()
    brisk.runTest()

    #sift_image = SIFT(image)
    # orb.extractFeatures(norm_type=cv2.NORM_HAMMING)

    # orb_image = orb.drawMatches()
    # plt.imshow(orb_image)
    # plt.show()

    # #orb.computeGroups()
    # #orb_image = orb.drawGroupMatches()
    # plt.imshow(orb_image)
    # plt.show()


    #freak_image = FREAK(image)

    
