#!/usr/bin/env python
import numpy as np
import cv2

# Returns list of matches that are greated that distance apart
def filter_distance(matches, keypoints, distance):

    return [match for match in matches if np.linalg.norm(np.array(keypoints[match.queryIdx].pt) - np.array(keypoints[match.trainIdx].pt)) > distance]



# Remove matchesToRemove from matches
def filter_matches(matches, matchesToRemove):

    return [match for match in matches if match not in matchesToRemove]

def get_color(seed):
    colors = [(255,0,0), (255,0,255), (255,0,128), (0,255,0), (0,255,255), (255,255,0), (128,255,0), (0,0,255)]

    return colors[seed % len(colors)]


def draw_matches(image, matches, keypoints):
    for idx, match in enumerate(matches):
        for m in match:
            pt1 = tuple(int(i) for i in keypoints[m.queryIdx].pt)
            pt2 = tuple(int(i) for i in keypoints[m.trainIdx].pt)
        
            image = cv2.line(image, pt1, pt2, color=get_color(idx))

    return image
