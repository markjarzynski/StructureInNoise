import cv2
import matplotlib.pyplot as plt

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
    plt.imshow(img3),plt.show()
    

if __name__ == "__main__":
    image = cv2.imread('./rng2img/city.png')
    SIFT(image)