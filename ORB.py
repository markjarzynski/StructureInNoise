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
        super().__init__(img, cv2.ORB_create, cv2.NORM_HAMMING)
        self.norm_type = cv2.NORM_HAMMING


def ORB(image):
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    detector = cv2.ORB_create()
    keypoints, descriptors = detector.detectAndCompute(gray, None)

    bf = cv2.BFMatcher(cv2.NORM_HAMMING, crossCheck=False)
    matches = bf.knnMatch(descriptors, descriptors, k=2)
    matches = [match for (_, match) in matches]

    matches = filter_distance(matches, keypoints, 5)

    _, _, filtered_matches = ransac(matches, keypoints, keypoints)

    group = [filtered_matches]
    m = matches

    while True:

        m = filter_matches(m, filtered_matches)
        _, _, filtered_matches = ransac(m, keypoints, keypoints)

        if len(filtered_matches) > 2:
            group.append(filtered_matches)
        else:
            break

    # debugging
    print("Total matches found:", len(matches))
    print("Number of keypoints:", len(keypoints))
    #print("Number of refined matches:", len(filtered_matches))
    print("Number of groups:", len(group))
    print("Group sizes:", ",".join([str(len(i)) for i in group]))

    #matched_image = cv2.drawMatches(image, keypoints, image, keypoints, filtered_matches[:30], None, flags=2)

    matched_image = draw_matches(image, group, keypoints)

    return matched_image

def usage(program):
    print(f"Usage: {program} <image>")

if __name__ == "__main__":

    if len(sys.argv) != 2:
        usage(sys.argv[0])
        exit()

    image = cv2.imread(sys.argv[1])

    if image is None:
        print("Error: Image did not load.")
        exit()

    image = ORB(image)
    plt.imshow(image),plt.show()
