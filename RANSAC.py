import numpy as np

def ransac(matches, kp1, kp2, iter=100, thres=5):

    # model variables
    max_inliers = 0
    best_tx, best_ty = 0, 0
    best_matches = []

    for _ in range(iter):
        # obtain translation coordinates
        match = matches[np.random.randint(0, len(matches))]
        pt1 = kp1[match.queryIdx].pt
        pt2 = kp2[match.trainIdx].pt
        tx, ty = (pt2[0] - pt1[0]), (pt2[1] - pt1[1])

        """
        This part compares keypoints within the same image, translated by a vector (tx,ty),
        against their original positions in that same image to determine if they qualify
        as inliers based on a distance threshold.
        """
        inliers = [
            np.linalg.norm(
                np.array((kp1[match.queryIdx].pt[0] + tx, kp1[match.queryIdx].pt[1] + ty))
                -
                np.array(kp2[match.trainIdx].pt)) < thres
            for match in matches
        ]

        # update model variables
        num_inliers = np.sum(inliers)
        if num_inliers > max_inliers:
            max_inliers = num_inliers
            best_tx, best_ty = tx, ty
            best_matches = [matches[i] for i, inlier in enumerate(inliers) if inlier]

    return best_tx, best_ty, best_matches
