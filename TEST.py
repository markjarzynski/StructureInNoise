#!/usr/bin/env python
import sys
import math
import numpy as np
import cv2

def usage():
    print(f"Usage: {sys.argv[0]} <image>")

def compare(a, b):
    if (a==b).all():
        print("True")

    return (a == b).all()


def find_matches(image, kernel, _x, _y):

    (height, width, _) = image.shape
    (h, w, _) = kernel.shape
    
    for x in range(0, width - w):
        for y in range(0, height - h):
            if not (x == _x and y == _y):
                crop = image[x:x+w, y:y+h]
                if compare(kernel, crop):
                    return (255, 255, 255)
    
    return (0,0,0)

if __name__ == "__main__":

    if len(sys.argv) != 2:
        usage()
        exit()

    image = cv2.imread(sys.argv[1])

    if image is None:
        print("Error: Image did not load.")
        exit()

    out_image = np.zeros(image.shape)

    s = 5
    
    (height, width, _) = image.shape
    
    for x in range(0, width - s):
        for y in range(0, height - s):
            kernel = image[x:x+s, y:y+s]
            print(kernel.shape, x,y)
            out_image[x, y] = find_matches(image, kernel, x, y)

    filename = "test.png"
    cv2.imwrite(filename, out_image)
