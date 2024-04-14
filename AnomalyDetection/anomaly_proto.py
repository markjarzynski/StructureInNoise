import numpy as np
import cv2
import pynear 		# VP Trees

import tensorflow as tf 	#Possibly unneeded


def patternMatching(input_img, patch_size, nearest_n, patch_similarity, disk_radii, nfa_thresh):

	''' Create multiscale decomposition of the image
		by taking the gaussian and down sampling a 
		finite number of iterations
	'''

	decomp_set = multiscaleDecomp(input_img, N_SCALES)

	''' 
		For each scaled image in the decomp list
		Compute Residual
		fitResidual
		detect image features
	'''

def constructVPTree(points, num_ele, bin_size):
	if len(points) <= bin_size:
		return None
	'''
	s = chooseVantage(points, num_ele)
	median_dist = computeMedianDistance(s, points)
	points_closer = []
	points_further = []
	for p in points:
		if median_dist > diff(p, s):
			points_closer.append(p)
		else:
			points_further.append(p)
	T1 = constructTree(points_closer)
	T2 = constructTree(points_futher)
	return (s, median_dist), T1, T2
	'''

def chooseVantagePoint(points, num_ele):

	s = 0
	'''
		b_set = selectPoints(points, num_ele)
		for p in b_set:
			computeMedianDistance(p, points)
			secMoment = computeSecondMoment(abs(p - u) - median_dist)
			s = s if s > secMoment else secMoment

	'''
	return s

def searchVantagePointTree(img, vp_tree, patch, search_size, patches_req, forbidden=None):
	'''
		Working
	'''

	pass

def computeResidual(img, img_ref, nn, sim_param, patch_size):

	'''
		# Algorithm 5 from paper
		for all P of u:
			estimateNNdeprivedP()
			reconstructPatch()

		u_hat = []
		for pixel in u:
			# Aggregate different estimates
			uhat[pixel] = sum
	'''

	pass

def fitResidual(r_set):
	'''
		# Fit the residual distribution and convert to Gaussian
	
		for all alpha in range(0.5, 1.4, 0.1):
			test_laplac = testLaplacian(r_set, alpha)
			test_gaus = testGaussian(r_set, alpha)
			
		

		# a stores index of best distibution
		return inv_cdf(cdf(r_set, a))
	'''
	pass

def multiscaleDecomp(img, n):
	decomp_set = []
	for i in range(n):
		new_shape = img_dwn.shape // 2
		img_dwn = cv2.GaussianBlur(img_dwn, 5, sigmaX=0.5)
		img_dwn = cv2.resize(img_dwn, new_shape)
		decomp_set.append(img_dwn)

	return decomp_set

def detectAnomalies(residual, img_size, disk_radii, nfa_thresh, patch_size):
		'''

		'''
		pass