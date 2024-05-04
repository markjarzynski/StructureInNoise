#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright (C) 2017, Axel Davy, CMLA, École Normale Supérieure de Cachan.
#
# This software is governed by the CeCILL-C license under French law and
# abiding by the rules of distribution of free software.  You can  use,
# modify and/ or redistribute the software under the terms of the CeCILL-C
# license as circulated by CEA, CNRS and INRIA at the following URL
# "http://www.cecill.info".
#
# As a counterpart to the access to the source code and  rights to copy,
# modify and redistribute granted by the license, users are provided only
# with a limited warranty  and the software's author,  the holder of the
# economic rights,  and the successive licensors  have only  limited
# liability.
#
# In this respect, the user's attention is drawn to the risks associated
# with loading,  using,  modifying and/or developing or reproducing the
# software by the user in light of its specific status of free software,
# that may mean  that it is complicated to manipulate,  and  that  also
# therefore means  that it is reserved for developers  and  experienced
# professionals having in-depth computer knowledge. Users are therefore
# encouraged to load and test the software's suitability as regards their
# requirements in conditions enabling the security of their systems and/or
# data to be ensured and,  more generally, to use and operate it in the
# same conditions as regards security.
#
# The fact that you are presently reading this means that you have had
# knowledge of the CeCILL-C license and that you accept its terms.
#
#

import argparse
import os
import numpy as np
import scipy.misc
import iio as piio

from load_network import load_network

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('layer_num', type=int, help=('num of the layer for VGG'))
    parser.add_argument('input', type=str, help=('path to source (png)'))
    parser.add_argument('output', type=str, help=('path to output directory'))
    parser.add_argument('demoExtras', type=str, help=('path to demoExtras directory'))
    args = parser.parse_args()

    network = load_network(args.demoExtras)
    img = piio.read(args.input)
    if len(img.shape)==2 or img.shape[2] == 1:
        new_img = np.zeros([img.shape[0], img.shape[1], 3], dtype=img.dtype)
        new_img[:,:,0] = img[:,:,0]
        new_img[:,:,1] = img[:,:,0]
        new_img[:,:,2] = img[:,:,0]
        img = new_img

    data = network.run_for_img(img, args.layer_num)
    data_ = np.array(data, order='C')
    data_[data_ != 0] /= np.sqrt(np.abs(data_[data_ != 0]))
    piio.write(os.path.join(args.output, 'nn_output.tiff'), data_)
