#!/usr/bin/env python
import sys
import cv2
import matplotlib.pyplot as plt

from FeatureDetector import FeatureDetector

class BRISKClass(FeatureDetector):
    
    def __init__(self, img):
        super().__init__(img, cv2.BRISK_create)
        self.norm_type = cv2.NORM_HAMMING

    def runTest(self):
        self.extractFeatures(self.norm_type)

        out_image = self.drawMatches()
        plt.imshow(out_image)
        plt.show()

        self.computeGroups()
        out_image = self.drawGroupMatches()
        plt.imshow(out_image)
        plt.show()


def BRISK(image):
    img1 = image.copy()
    img2 = image.copy()
    
    gray = cv2.cvtColor(img1, cv2.COLOR_BGR2GRAY)
    gray2 = cv2.cvtColor(img2, cv2.COLOR_BGR2GRAY)
    
    brisk = cv2.BRISK_create()
    
    kp1, des1 = brisk.detectAndCompute(gray,None)
    kp2, des2 = brisk.detectAndCompute(gray2,None)
    
    bf = cv2.BFMatcher()
    
    matches = bf.knnMatch(des1, des2, k=2)
    matches = [y for (x,y) in matches]

    img3 = cv2.drawMatches(img1, kp1, img2, kp2, matches[:50], img2, flags=2) 

    return img3

    plt.imshow(image),plt.show()

def usage():
    print(f"Usage: {sys.argv[0]} <image>")

if __name__ == "__main__":

    if len(sys.argv) != 2:
        usage()
        exit()

    image = cv2.imread(sys.argv[1])

    brisk = BRISKClass(image)
    brisk.runTest()
    #image = BRISK(image)
    #cv2.imwrite('BRISK_out.png', image) 
