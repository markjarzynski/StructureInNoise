import cv2
import matplotlib.pyplot as plt


from FeatureDetector import FeatureDetector

class SIFTClass(FeatureDetector):
    
    def __init__(self, img):
        super().__init__(img, cv2.SIFT_create)
        self.norm_type = cv2.NORM_L1

    def runTest(self):
        self.extractFeatures(self.norm_type)

        out_image = self.drawMatches()
        plt.imshow(out_image)
        plt.show()

        self.computeGroups()
        out_image = self.drawGroupMatches()
        plt.imshow(out_image)
        plt.show()

def SIFT(image):
    img1 = image.copy()
    img2 = image.copy()
    
    gray = cv2.cvtColor(img1, cv2.COLOR_BGR2GRAY)
    gray2 = cv2.cvtColor(img2, cv2.COLOR_BGR2GRAY)
    
    sift = cv2.SIFT_create()
    
    kp1, des1 = sift.detectAndCompute(gray,None)
    kp2, des2 = sift.detectAndCompute(gray2,None)
    
    bf = cv2.BFMatcher()
    
    matches = bf.knnMatch(des1, des2, k=2)
    matches = [y for (x,y) in matches]

    img3 = cv2.drawMatches(img1, kp1, img2, kp2, matches[:50], img2, flags=2) 

    return img3

if __name__ == "__main__":
    image = cv2.imread('./rng2img/city.png')
    image = SIFT(image)
    plt.imshow(image),plt.show()
