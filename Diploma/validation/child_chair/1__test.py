import os
import sys
sys.path.append(os.path.abspath(os.path.join('../../src')))

sys.path.append(os.path.abspath(os.path.join('../')))

import numpy

train_features = numpy.hstack([
    numpy.load(f"1/{media_code}/train_features.npy")
    for media_code in [ "front", "label", "side" ]
])

test_features = numpy.hstack([
    numpy.load(f"1/{media_code}/test_features.npy")
    for media_code in [ "front", "label", "side" ]
])

print(train_features.shape, test_features.shape)

train_front_labels = numpy.load("1/front/train_labels.npy")
train_label_labels = numpy.load("1/label/train_labels.npy")
train_side_labels = numpy.load("1/side/train_labels.npy")
assert (train_front_labels == train_label_labels).all()
assert (train_front_labels == train_side_labels).all()
train_labels = train_front_labels
print(train_labels.shape)

test_front_labels = numpy.load("1/front/test_labels.npy")
test_label_labels = numpy.load("1/label/test_labels.npy")
test_side_labels = numpy.load("1/side/test_labels.npy")
assert (test_front_labels == test_label_labels).all()
assert (test_front_labels == test_side_labels).all()
test_labels = test_front_labels
print(test_labels.shape)

import torch
from utils.classify import classify

classify(
    [
        numpy.load("1/front/train_images.npy"),
        numpy.load("1/label/train_images.npy"),
        numpy.load("1/side/train_images.npy")
    ],
    train_features,
    train_labels,
    
    [
        numpy.load("1/front/test_images.npy"),
        numpy.load("1/label/test_images.npy"),
        numpy.load("1/side/test_images.npy")
    ],
    test_features,
    test_labels,
    
    nn_device = torch.device('cuda'), dump_name = "1/predictions",
    catboost_device = 'CPU', xgboost_device = 'cpu',
    label_names = [ "fail", "success" ], # , only_get_from_dump = True
)
