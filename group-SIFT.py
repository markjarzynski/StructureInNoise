#!/usr/bin/env python
import cv2
import matplotlib.pyplot as plt
import sys


from FeatureDetector import FeatureDetector

class SIFTClass(FeatureDetector):
    
    def __init__(self, img):
        super().__init__(img, cv2.SIFT_create)
        self.norm_type = cv2.NORM_L1
        self.name = "SIFT"

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

    sift.extractFeatures()
    sift.computeGroup()
    sift.printGroup()
