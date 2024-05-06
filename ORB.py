#!/usr/bin/env python
import sys
import cv2
import numpy as np
import matplotlib.pyplot as plt

from RANSAC import ransac
from util import *
from FeatureDetector import FeatureDetector

class ORBClass(FeatureDetector):
    
    def __init__(self, img):
        super().__init__(img, cv2.ORB_create)
        self.norm_type = cv2.NORM_HAMMING
        self.name = "ORB"

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

    orb = ORBClass(image)
    orb.run()
    
    orb.printFirst()
    orb.writeFristImage()
