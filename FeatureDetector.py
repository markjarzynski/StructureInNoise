import cv2
import numpy as np
import matplotlib.pyplot as plt

from RANSAC import ransac
from util import *

DEBUGGING = True
RANSAC_LOOP_MAX = 40

class FeatureDetector():
    '''
        Initialize a FeatureDetector object with inputs of
        img - Image to detect features within
        technique - Feature matching model
    '''
    def __init__(self, img, technique):
        self.img = img.copy()
        self.technique = technique
        self.matches = None
        self.first = None
        self.group = None
        self.kpts = None
        self.iter = 100
        self.norm_type=cv2.NORM_L1
        self.name = None


    def extractFeatures(self):
        gray = cv2.cvtColor(self.img, cv2.COLOR_BGR2GRAY)
        
        detector = self.technique()

        self.kpts, descriptors = detector.detectAndCompute(gray, None)

        bf = cv2.BFMatcher(self.norm_type, crossCheck=False)
        matches = bf.knnMatch(descriptors, descriptors, k=2)

        try:
        	self.matches = [match for (_, match) in matches]
        	self.matches = filter_distance(self.matches, self.kpts, 5)
        except ValueError:
        	return False

    def computeFirst(self):
        if 0 == len(self.matches):
            if DEBUGGING:
                print("No matches found, can't compute group")
            self.first = []
            return False

        _, _, self.first = ransac(self.matches, self.kpts, self.kpts, self.iter)
        self.group = [self.first]

    def computeGroup(self):

        if 0 == len(self.matches):
            if DEBUGGING:
                print("No matches found, can't compute group")
            return False

        _, _, filtered_matches = ransac(self.matches, self.kpts, self.kpts, self.iter)

        try:
	        self.group.append(filter_matches)
        except NameError:
        	self.group = [filtered_matches]

        m = list(self.matches)

        remainging = RANSAC_LOOP_MAX

        while True and remainging:
            m = filter_matches(m, filtered_matches)
            _, _, filtered_matches = ransac(m, self.kpts, self.kpts, self.iter)

            print(len(m), len(filtered_matches))
            if len(filtered_matches) > 2:
                self.group.append(filtered_matches)
            else:
                break
            remainging -= 1

        # debugging
        if DEBUGGING:
            print("Total matches found:", len(self.matches))
            print("Number of keypoints:", len(self.kpts))
            #print("Number of refined matches:", len(filtered_matches))
            print("Number of groups:", len(self.group))
            print("Group sizes:", ",".join([str(len(i)) for i in self.group]))

    def run(self):
        self.extractFeatures()
        self.computeFirst()

    def drawFirst(self):
        image = None
        for idx, match in enumerate(self.first):
            pt1 = tuple(int(i) for i in self.kpts[match.queryIdx].pt)
            pt2 = tuple(int(i) for i in self.kpts[match.trainIdx].pt)

            image = cv2.line(self.img, pt1, pt2, color=get_color(idx))

        return image
    
    def drawGroupMatches(self):
        for idx, matches in enumerate(self.group):
            for match in matches:
                pt1 = tuple(int(i) for i in keypoints[match.queryIdx].pt)
                pt2 = tuple(int(i) for i in keypoints[match.trainIdx].pt)

                image = cv2.line(self.img, pt1, pt2, color=get_color(idx))

        return image

    def printFirst(self):
        print(f"{self.name}: {len(self.first)}")

    def printGroup(self):
        print(self.name, "Group:", ",".join([str(len(i)) for i in self.group]))
