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
		self.group = None
		self.kpts = None

	def extractFeatures(self, norm_type=cv2.NORM_L1):
		gray = cv2.cvtColor(self.img, cv2.COLOR_BGR2GRAY)
		
		detector = self.technique()

		self.kpts, descriptors = detector.detectAndCompute(gray, None)

		bf = cv2.BFMatcher(norm_type, crossCheck=False)
		matches = bf.knnMatch(descriptors, descriptors, k=2)
		self.matches = [match for (_, match) in matches]
		self.matches = filter_distance(self.matches, self.kpts, 5)

	def computeGroups(self):

		_, _, filtered_matches = ransac(self.matches, self.kpts, self.kpts)

		self.group = [filtered_matches]
		m = list(self.matches)

		remainging = RANSAC_LOOP_MAX

		while True and remainging:
			m = filter_matches(m, filtered_matches)
			_, _, filtered_matches = ransac(m, self.kpts, self.kpts)

			print(len(filtered_matches))
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

	def drawMatches(self):
		return cv2.drawMatches(self.img, self.kpts, self.img, self.kpts, self.matches[:30], None, flags=2)
	
	def drawGroupMatches(self):
		return draw_matches(self.img, self.group, self.kpts)