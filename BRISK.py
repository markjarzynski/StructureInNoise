#!/usr/bin/env python
import sys
import cv2

from FeatureDetector import FeatureDetector

class BRISKClass(FeatureDetector):
    
    def __init__(self, img, name):
        super().__init__(img, cv2.BRISK_create)
        self.norm_type = cv2.NORM_HAMMING
        self.iter = 500
        self.name = name
        self.method = 'BRISK'

def usage():
    print(f"Usage: {sys.argv[0]} <image>")

if __name__ == "__main__":

    if len(sys.argv) != 2:
        usage()
        exit()

    basename = os.path.basename(filename)
    parts = basename.split('.')
    name = parts[0] 

    image = cv2.imread(sys.argv[1])

    if image is None:
        print("Error: Image did not load.")
        exit()

    brisk = BRISKClass(image, name)
    brisk.run()

    brisk.printFirst()
    brisk.writeFirstImage()
