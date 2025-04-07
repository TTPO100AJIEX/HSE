import os
import sys
sys.path.append(os.path.abspath(os.path.join('../../cvtda')))

sys.path.append(os.path.abspath(os.path.join('../')))

import numpy

train_features = numpy.hstack([
    numpy.load(f"1/{media_code}/train_features.npy")
    for media_code in [ "lightbox_left", "lightbox_right", "front" ]
])

test_features = numpy.hstack([
    numpy.load(f"1/{media_code}/test_features.npy")
    for media_code in [ "lightbox_left", "lightbox_right", "front" ]
])

print(train_features.shape, test_features.shape)

train_labels = numpy.load("1/front/train_labels.npy")
print(train_labels.shape)

test_labels = numpy.load("1/front/test_labels.npy")
print(test_labels.shape)

import torch
from utils.classify import classify

classify(
    None,
    train_features,
    train_labels,
    
    None,
    test_features,
    test_labels,
    
    nn_device = torch.device('cpu'), dump_name = "1/predictions",
    catboost_device = 'CPU', xgboost_device = 'cpu'# , only_get_from_dump = True
)
