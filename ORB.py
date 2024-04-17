import cv2
import numpy as np
import matplotlib.pyplot as plt

def ORB(image):
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    detector = cv2.ORB_create()
    keypoints, descriptors = detector.detectAndCompute(gray, None)

    bf = cv2.BFMatcher(cv2.NORM_HAMMING, crossCheck=False)
    matches = bf.knnMatch(descriptors, descriptors, k=2)

    filtered_matches = []
    min_distance = 70  # set minimum pixel distance to filter trivial self-matches

    filtered_matches = [
        n for (m, n) in matches if # ignore first match in knn as it is always a self-match
        np.linalg.norm(np.array(keypoints[n.queryIdx].pt) - np.array(keypoints[n.trainIdx].pt)) >= min_distance
    ]

    # debugging
    print("Total matches found:", len(matches))
    print("Number of keypoints:", len(keypoints))
    print("Number of refined matches:", len(filtered_matches))

    matched_image = cv2.drawMatches(image, keypoints, image, keypoints, filtered_matches[:30], None, flags=2)
    return matched_image

if __name__ == "__main__":
    image = cv2.imread('./rng2img/Hash Images/esgtsa_linear.png')

    if image is None:
        print("Error: Image did not load.")
    else:
        image = ORB(image)
        plt.imshow(image),plt.show()
