import cv2
import numpy as np

from RANSAC import ransac
from util import *

DEBUGGING = False
RANSAC_LOOP_MAX = 20

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
        self.method = None
        self.H = []


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

        H, self.first = ransac(self.matches, self.kpts, self.kpts, self.iter)
        self.group = [self.first]
        self.H = [H]

    def computeGroup(self):

        if 0 == len(self.matches):
            if DEBUGGING:
                print("No matches found, can't compute group")
            return False

        H, filtered_matches = ransac(self.matches, self.kpts, self.kpts, self.iter)

        self.first = filtered_matches
        self.group = [filtered_matches]
        self.H = [H]

        m = list(self.matches)

        remainging = RANSAC_LOOP_MAX

        while True and remainging:
            m = filter_matches(m, filtered_matches)
            H, filtered_matches = ransac(m, self.kpts, self.kpts, self.iter)

            if DEBUGGING:
                print(len(m), len(filtered_matches))

            if len(filtered_matches) > 2:
                self.group.append(filtered_matches)
                self.H.append(H)
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
        self.computeGroup()

    def drawFirst(self):
        image = None
        for idx, match in enumerate(self.first):
            pt1 = tuple(int(i) for i in self.kpts[match.queryIdx].pt)
            pt2 = tuple(int(i) for i in self.kpts[match.trainIdx].pt)

            image = cv2.line(self.img, pt1, pt2, color=(0,0,0), thickness=4, lineType=cv2.LINE_AA)
            image = cv2.line(image, pt1, pt2, color=get_color(idx), thickness=2, lineType=cv2.LINE_AA)

        return image
    
    def drawGroupMatches(self):
        for idx, matches in enumerate(self.group):
            for match in matches:
                pt1 = tuple(int(i) for i in keypoints[match.queryIdx].pt)
                pt2 = tuple(int(i) for i in keypoints[match.trainIdx].pt)

                image = cv2.line(self.img, pt1, pt2, color=get_color(idx))

        return image

    def printFirst(self):
        print(f"{self.name} {self.method}: {len(self.first)}")

    def printGroup(self):
        print(f"{self.name},{self.method}," + ",".join([str(len(i)) for i in self.group]))

    def writeFirstImage(self, filename=None):
        if not filename:
            filename = f'{self.name}.{self.method}.png'

        try:
            cv2.imwrite(filename, self.drawFirst())
        except cv2.error:
            print("Unable to output image with no matches")

    def writeMatchedImage(self, filename=None, h=9, w=9):
        if not filename:
            filename = f'{self.name}.{self.method}.cropped.png'

        if not self.first:
            print("Unable to create cropped match image. No matches found.")
            return

        seperator_x = np.zeros([h, 1, 3])
        seperator_y = np.zeros([1, (w + 1) * 3, 3])

        output_img = np.zeros(shape=[1, (w + 1) * 3, 3])

        for match in self.matches:

            #match = self.first[0]
            
            pt1 = tuple(int(i) for i in self.kpts[match.queryIdx].pt)
            pt2 = tuple(int(i) for i in self.kpts[match.trainIdx].pt)

            # crop the image
            cropped_img1 = self.img[pt1[1]:pt1[1]+h, pt1[0]:pt1[0]+w]
            cropped_img2 = self.img[pt2[1]:pt2[1]+h, pt2[0]:pt2[0]+w]

            # add seperators
            cropped_img1 = np.concatenate((cropped_img1, seperator_x), axis=1)
            cropped_img2 = np.concatenate((cropped_img2, seperator_x), axis=1) 

            cropped_img = np.concatenate((cropped_img1, cropped_img2), axis=1)

            H = self.H[0]

            origin = np.matmul(H, np.array([[0], [0], [1]]))
            large = np.matmul(H, np.array([[256], [256], [1]]))
            min_x = min(origin[0], large[0])
            max_x = max(origin[0], large[0])
            min_y = min(origin[1], large[1])
            max_y = max(origin[1], large[1])

            shape = (int(max_y - min_y), int(max_x - min_x))
            print(shape)
            #warped_img = cv2.warpPerspective(self.img, H, shape)
            warped_img = cv2.warpAffine(self.img, H[:2], shape)

            print("warped img shape: ", warped_img.shape)

            warped_pt = np.matmul(H, np.array([[pt2[0]], [pt2[1]], [1]]))
            print(warped_pt)

            #warped_pt = [int(warped_pt[0] / warped_pt[2]), int(warped_pt[1] / warped_pt[2])]
            warped_pt = [int(warped_pt[0] + 0.5), int(warped_pt[1] + 0.5)]
            print(warped_pt)

            if (warped_pt[0] < w or warped_pt[0] > 256 - w or warped_pt[1] < h or warped_pt[1] > 256 - h):
                continue

            print(warped_pt[1] + h, warped_pt[0] + w)
            cropped_warped_img = warped_img[warped_pt[1]:warped_pt[1]+h, warped_pt[0]:warped_pt[0]+w]
            print(cropped_warped_img.shape)
            cropped_warped_img = np.concatenate((cropped_warped_img, seperator_x), axis=1) 
            
            cropped_img = np.concatenate((cropped_img, cropped_warped_img), axis=1)
            print(cropped_img.shape, output_img.shape)
        
            cropped_img = np.concatenate((cropped_img, seperator_y), axis=0)
            output_img = np.concatenate((output_img, cropped_img), axis=0)

        try:
            cv2.imwrite(filename, output_img)
        except:
            print("Unable to output cropped match image.")


