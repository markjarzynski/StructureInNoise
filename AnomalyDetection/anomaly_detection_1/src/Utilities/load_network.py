#!/usr/bin/env python
# Code inspired from https://github.com/leongatys/DeepTextures/blob/master/DeepImageSynthesis/Misc.py
# which had the mention "This software is published for academic and non-commercial use only."

import caffe
import numpy as np

class network:
    def __init__(self, bin_path, proto_path, name, visible_layers, train_mean):
        self.bin_path = bin_path
        self.proto_path = proto_path
        self.name = name
        self.visible_layers = visible_layers
        self.train_mean = train_mean

    def run_for_img(self, img, layer_num):
        with open(self.proto_path,'r+') as f:
            data = f.readlines() 
        data[2] = "input_dim: 1\n"
        data[3] = "input_dim: 3\n"
        data[4] = "input_dim: %i\n" %(img.shape[0])
        data[5] = "input_dim: %i\n" %(img.shape[1])
        with open(self.proto_path,'r+') as f:
            f.writelines(data)
        net_mean =  np.tile(self.train_mean[:,None,None],(1,) + (img.shape[0], img.shape[1]))
        net = caffe.Classifier( 
            self.proto_path, self.bin_path,
            mean = net_mean,
            channel_swap=(2,1,0),
            input_scale=255,)
        img_pp = net.transformer.preprocess('data',img)[None,:]
        net.forward(data = img_pp)
        layer = self.visible_layers[layer_num]
        data = net.blobs[layer].data
        data = data.reshape(*net.blobs[layer].data.shape)[0].transpose(1,2,0)[:,:,::-1]
        return data


def load_network(demoExtras):
    return network(demoExtras+'/models/vgg_gatys/VGG_normalised.caffemodel',\
                   demoExtras+'/models/vgg_gatys/deploy.prototxt', 'VGG',\
                   ['conv1_1', 'conv1_2', 'pool1',\
                    'conv2_1', 'conv2_2', 'pool2',\
                    'conv3_1', 'conv3_2', 'conv3_3', 'conv3_4', 'pool3',\
                    'conv4_1', 'conv4_2', 'conv4_3', 'conv4_4'],\
                   np.array([ 0.40760392,  0.45795686,  0.48501961]))
