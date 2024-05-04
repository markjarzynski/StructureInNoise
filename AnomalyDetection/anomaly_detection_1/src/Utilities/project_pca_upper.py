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
import os.path
import numpy as np
import scipy.misc
import iio as piio
import errno
import scipy.linalg
from scipy.stats import binom

def PCA_upper(data_candidate, data_ref):
    m, n = data_ref.shape
    # Use high precision
    data_ref = np.asarray(data_ref, dtype=np.float64)
    # Remove 1st order statistics
    data_candidate -= data_ref.mean(axis=0)
    data_ref -= data_ref.mean(axis=0)
    C = np.cov(data_ref, rowvar=False)
    D, eigs = scipy.linalg.eigh(C)
    # Remove low precision eigenvalues
    # D is in ascending order.
    D[D < 1e-15 * D[-1]] = 0
    # PCA: C^-1/2 = D^-1/2 E' D:values, E = Vectors
    # ZCA: C^-1/2 = E D^-1/2 E'
    num_channels = min(5, min(n//2, sum(D>0)))
    C_m_1_2 = np.zeros([num_channels, n])
    for c0 in range(num_channels):
        c = c0 + (n - num_channels)
        assert (D[c] > 0)
        C_m_1_2[c0,:] = (1./np.sqrt(D[c])) * eigs[:,c]
    return np.dot(C_m_1_2, data_candidate.T).T, np.dot(C_m_1_2, data_ref.T).T

def pca_on_img(data_candidate, data_ref):
    [n, m, c] = data_ref.shape
    [nc, mc, _] = data_candidate.shape
    if (n*m < c):
        # cannot do PCA
        return data_candidate
    data_ref = data_ref.reshape([n * m, c])
    data_candidate = data_candidate.reshape([nc * mc, c])
    pca, _ = PCA_upper(data_candidate, data_ref)
    new_c = pca.shape[1]
    pca = pca.reshape([nc, mc, new_c])
    return pca

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('input', type=str, help=('path to source (tiff)'))
    parser.add_argument('output', type=str, help=('path to output directory'))
    args = parser.parse_args()

    print args.input

    img = piio.read(args.input)
    img = pca_on_img(img, img)
    img = np.array(img, order='C')
    piio.write(os.path.join(args.output, os.path.basename(args.input)), img)
 
