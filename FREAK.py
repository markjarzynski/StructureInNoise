import sys
import cv2
import matplotlib.pyplot as plt

from FeatureDetector import FeatureDetector
from util import * 

class FREAKClass(FeatureDetector):

    def __init__(self, img):
        super().__init__(img, cv2.xfeatures2d.FREAK_create)
        self.norm_type = cv2.NORM_L1

    def extractFeatures(self, norm_type=cv2.NORM_L1):
        gray = cv2.cvtColor(self.img, cv2.COLOR_BGR2GRAY)
        
        fast = cv2.FastFeatureDetector_create()
        detector = self.technique()

        kpts = fast.detect(gray, None)
        self.kpts, descriptors = detector.compute(gray, kpts)
        
        bf = cv2.BFMatcher(norm_type, crossCheck=False)
        matches = bf.knnMatch(descriptors, descriptors, k=2)
        self.matches = [match for (_, match) in matches]
        self.matches = filter_distance(self.matches, self.kpts, 5)

    def runTest(self):
        self.extractFeatures(self.norm_type)

        out_image = self.drawMatches()
        plt.imshow(out_image)
        plt.show()

        self.computeGroups()
        out_image = self.drawGroupMatches()
        plt.imshow(out_image)
        plt.show()


def FREAK(image):
    img1 = image.copy()
    img2 = image.copy()
    
    gray1 = cv2.cvtColor(img1, cv2.COLOR_BGR2GRAY)
    gray2 = cv2.cvtColor(img2, cv2.COLOR_BGR2GRAY)
    
    fast = cv2.FastFeatureDetector_create()
    freak = cv2.xfeatures2d.FREAK_create()
    
    kp1 = fast.detect(gray1, None)
    kp2 = fast.detect(gray2, None)

    kp1, des1 = freak.compute(gray1, kp1)
    kp2, des2 = freak.compute(gray2, kp2)
    
    bf = cv2.BFMatcher()
    
    matches = bf.knnMatch(des1, des2, k=2)
    matches = [y for (x,y) in matches]

    img3 = cv2.drawMatches(img1, kp1, img2, kp2, matches[:50], img2, flags=2) 

    return img3

    plt.imshow(image),plt.show()

def usage():
    print("Usage: ./main.py <image>")

if __name__ == "__main__":

    if len(sys.argv) != 2:
        usage()
        exit()

    image = cv2.imread(sys.argv[1])
    image = FREAK(image)
    cv2.imwrite('FREAK_out.png', image) 
