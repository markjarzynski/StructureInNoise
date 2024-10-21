#!/usr/bin/env python
import sys
import cv2

from FeatureDetector import FeatureDetector
from util import * 

class FREAKClass(FeatureDetector):

    def __init__(self, img):
        super().__init__(img, cv2.xfeatures2d.FREAK_create)
        self.norm_type = cv2.NORM_L1
        self.iter = 500
        self.name = "FREAK"

    def extractFeatures(self):

        gray = cv2.cvtColor(self.img, cv2.COLOR_BGR2GRAY)
        
        fast = cv2.FastFeatureDetector_create()
        detector = self.technique()

        kpts = fast.detect(gray, None)
        self.kpts, descriptors = detector.compute(gray, kpts)
        
        bf = cv2.BFMatcher(self.norm_type, crossCheck=False)
        matches = bf.knnMatch(descriptors, descriptors, k=2)
        self.matches = [match for (_, match) in matches]
        self.matches = filter_distance(self.matches, self.kpts, 5)


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

    freak = FREAKClass(image)
    freak.run()

    freak.printFirst()

    freak.writeFirstImage()
