#!/usr/bin/env python
# -*- coding: utf-8 -*-
#/*
# * Copyright (c) 2017, Axel Davy, CMLA, École Normale Supérieure de Cachan.
# * All rights reserved.
# *
# * This program is free software: you can use, modify and/or
# * redistribute it under the terms of the GNU General Public
# * License as published by the Free Software Foundation, either
# * version 3 of the License, or (at your option) any later
# * version. You should have received a copy of this license along
# * this program. If not, see <http://www.gnu.org/licenses/>.
# */

import argparse
import os
import numpy as np
import scipy.misc
import scipy.stats
import iio as piio

hist_resolution = 20
hist_step = 1./hist_resolution

from scipy.stats import norm, laplace

# scipy laplace.sf gives same results than 1-laplace.cdf,
# which gives disappointing accuracy at the tail.
def laplace_sf(arr, scale):
    res = np.zeros(arr.shape, dtype=arr.dtype)
    res[arr > 0] = 0.5 * np.exp(-arr[arr > 0]/scale)
    res[arr <= 0] = 1-0.5 * np.exp(arr[arr <= 0]/scale)
    return res

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('input', type=str, help=('path to source (tiff)'))
    parser.add_argument('output', type=str, help=('path to output (tiff)'))
    args = parser.parse_args()
    print (args.input)

    img = piio.read(args.input)
    img = np.array(img, order='C')
    for c in range(img.shape[2]):
        arr = img[:,:,c].flatten()
        if np.sum(arr!=0) == 0:
            continue

        # We search the best distribution parameters among a fixed list.
        best_score = np.inf
        best_alpha = 1
        best_mode = ''
        best_data = arr

        # Is arr^alpha a Laplacian
        for alpha in [0.5, 0.6, 0.7, 0.8, 0.9, 1, 1.1, 1.2, 1.3, 1.4]:
            # data <- arr^alpha
            data = np.sign(arr) * np.abs(arr)**alpha
            # Ignore zeros for the parameter estimation (when using CNN
            # features, RELUs can cause a lot of zeros which are not
            # relevant for the distribution estimation.
            scale = np.sqrt(data[data!=0].var()/2.)
            # If all black, do not generate NaNs
            if scale == 0:
                scale = 1
            # Apply the Laplacian cdf on data.
            # If data follows a Laplacian, we should get
            # an uniform random variable.
            data_cdf = laplace.cdf(data, loc=0, scale=scale)
            # Ignore zeros
            data_cdf_cut = data_cdf[data!=0]
            # Produce a normalized histogram of data_cdf (ignoring zeros)
            hist,_ = np.histogram(data_cdf_cut, range=(0., 1.), bins=hist_resolution)
            hist = hist / float(data_cdf_cut.shape[0])
            # score if the squared L2 distance between the cumulated histograms
            # of data_cdf and an uniform random variable.
            score = np.sum((np.cumsum(hist)-np.cumsum(hist_step * np.ones(hist.shape[0])))**2)

            if score < best_score:
                best_score = score
                # We apply the Gaussian distribution quantile function
                # To convert the uniformly distributed data_cdf
                # To a standard Gaussian.
                # For precision, we use sf and isf for positive values.
                best_data = np.zeros_like(data)

                data_sf = norm.isf(laplace_sf(data, scale))
                mask = data_sf > 0
                best_data[mask] = data_sf[mask]
                mask2 = np.logical_not(mask)
                best_data[mask2] = norm.ppf(data_cdf)[mask2]

                best_alpha = alpha
                best_mode = 'laplacian'

        # Is arr^alpha a Gaussian
        for alpha in [0.5, 0.6, 0.7, 0.8, 0.9, 1, 1.1, 1.2, 1.3, 1.4]:
            # data <- arr^alpha
            data = np.sign(arr) * np.abs(arr)**alpha
            # Ignore zeros for the parameter estimation (when using CNN
            # features, RELUs can cause a lot of zeros which are not
            # relevant for the distribution estimation.
            scale = np.sqrt(data[data!=0].var())
            # If all black, do not generate NaNs
            if scale == 0:
                scale = 1
            # Apply the Gaussian cdf on data.
            # If data follows a Gaussian, we should get
            # an uniform random variable.
            data_cdf = norm.cdf(data, loc=0, scale=scale)
            # Ignore zeros
            data_cdf_cut = data_cdf[data!=0]
            # Produce a normalized histogram of data_cdf (ignoring zeros)
            hist,_ = np.histogram(data_cdf_cut, range=(0., 1.), bins=hist_resolution)
            hist = hist / float(data_cdf_cut.shape[0])
            # score if the squared L2 distance between the cumulated histograms
            # of data_cdf and an uniform random variable.
            score = np.sum((np.cumsum(hist)-np.cumsum(hist_step * np.ones(hist.shape[0])))**2)

            if score < best_score:
                best_score = score
                # Keep the proposed Gaussian variable
                best_data = data/scale
                best_alpha = alpha
                best_mode = 'gaussian'

        print('best is ', best_mode, ' ', best_alpha, ' ', best_score)
        
        arr = best_data
        # If never we have generated Infs and NaNs, ignore them
        arr[np.isinf(arr)] = 0
        arr[np.isnan(arr)] = 0
        
        arr = arr.reshape(img.shape[:2])
        img[:,:,c] = arr[:,:]

    piio.write(args.output, img)
 
