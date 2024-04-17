import sys
import cv2
import matplotlib.pyplot as plt

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
