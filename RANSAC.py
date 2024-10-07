import numpy as np
import cv2

def transform_point(H, pt):
    (x, y) = pt
    M = np.matmul(H, np.array([[x],[y],[1]]))
    return M[0][0]/M[2][0], M[1][0]/M[2][0]

def distanceSq(a, b):
    ((x1, y1), (x2, y2)) = a, b
    return (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2)

def ransac(matches, kp1, kp2, iter=100, thres=5):

    thresSq = thres*thres

    # model variables
    max_inliers = 0
    #best_tx, best_ty = 0, 0
    best_H = np.zeros(shape=(4,4))
    best_matches = []

    for _ in range(iter):
        """
        # obtain translation coordinates
        match = matches[np.random.randint(0, len(matches))]
        pt1 = kp1[match.queryIdx].pt
        pt2 = kp2[match.trainIdx].pt
        tx, ty = (pt2[0] - pt1[0]), (pt2[1] - pt1[1])
        """

        """
        This part compares keypoints within the same image, translated by a vector (tx,ty),
        against their original positions in that same image to determine if they qualify
        as inliers based on a distance threshold.
        """
        """
        inliers = [
            np.linalg.norm(
                np.array((kp1[match.queryIdx].pt[0] + tx, kp1[match.queryIdx].pt[1] + ty))
                -
                np.array(kp2[match.trainIdx].pt)) < thres
            for match in matches
        ]
        """
        n = 4
        im1_pts = np.zeros(shape=(n, 2))
        im2_pts = np.zeros(shape=(n, 2))
        for i in range(0, n):
            match = matches[np.random.randint(0, len(matches))]
            im1_pts[i] = kp1[match.queryIdx].pt
            im2_pts[i] = kp2[match.trainIdx].pt
 
        H, mask = cv2.findHomography(im1_pts, im2_pts, method=cv2.RANSAC)
    
        try: 
            inv_H = np.linalg.inv(H)
        except:
            continue

        transformed = [(kp1[match.queryIdx].pt, transform_point(inv_H, kp2[match.trainIdx].pt)) for match in matches]

        errorSq = [distanceSq(a,b) for a,b in transformed]
        
        inliers = [True if e < thresSq else False for e in errorSq]

        # update model variables
        num_inliers = np.sum(inliers)
        if num_inliers > max_inliers:
            max_inliers = num_inliers
            #best_tx, best_ty = tx, ty
            best_H = H
            best_matches = [matches[i] for i, inlier in enumerate(inliers) if inlier]

    #return best_tx, best_ty, best_matches
    return best_H, best_matches
