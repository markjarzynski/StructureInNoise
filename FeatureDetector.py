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
	def __init__(self, img, technique, norm_type=cv2.NORM_L1):
		self.img = img.copy()
		self.technique = technique
		self.norm_type = norm_type
		self.matches = None
		self.group = None
		self.kpts = None
		self.iter = 100

	def extractFeatures(self, norm_type=cv2.NORM_L1):
		gray = cv2.cvtColor(self.img, cv2.COLOR_BGR2GRAY)
		
		detector = self.technique()

		self.kpts, descriptors = detector.detectAndCompute(gray, None)

		bf = cv2.BFMatcher(norm_type, crossCheck=False)
		matches = bf.knnMatch(descriptors, descriptors, k=2)

		try:
			self.matches = [match for (_, match) in matches] 
			self.matches = filter_distance(self.matches, self.kpts, 5)
		except ValueError:
			return

	def computeGroups(self):

		if not self.matches:
			return False

		_, _, filtered_matches = ransac(self.matches, self.kpts, self.kpts, iter=self.iter)

		self.group = [filtered_matches]
		m = list(self.matches)

		remaining = RANSAC_LOOP_MAX

		while True and remaining:
			m = filter_matches(m, filtered_matches)
			_, _, filtered_matches = ransac(m, self.kpts, self.kpts, iter=self.iter)

			print(len(filtered_matches))
			if len(filtered_matches) > 2:
				self.group.append(filtered_matches)
			else:
				break
			remaining -= 1

        # debugging
		if DEBUGGING:
			print("Total matches found:", len(self.matches))
			print("Number of keypoints:", len(self.kpts))
			#print("Number of refined matches:", len(filtered_matches))
			print("Number of groups:", len(self.group))
			print("Group sizes:", ",".join([str(len(i)) for i in self.group]))

		return True

	def drawMatches(self):
		if not self.matches:
			return None

		return cv2.drawMatches(self.img, self.kpts, self.img, self.kpts, self.matches[:30], None, flags=2)
	
	def drawGroupMatches(self):
		if not self.group:
			return None

		return draw_matches(self.img, self.group, self.kpts)

	def runTest(self):
		self.extractFeatures(self.norm_type)

		out_image = self.drawMatches()
		if out_image:
			plt.imshow(out_image)
			plt.show()

		self.computeGroups()
		out_image = self.drawGroupMatches()
		if out_image:
			plt.imshow(out_image)
			plt.show()