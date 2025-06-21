import os
import sys
sys.path.append(os.path.abspath(os.path.join('../../src')))

import numpy

train_images = numpy.load("train_images.npy")
test_images = numpy.load("test_images.npy")

train_labels = numpy.load("train_labels_number_visible.npy")
test_labels = numpy.load("test_labels_number_visible.npy")

train_features = numpy.load("train_features.npy")
test_features = numpy.load("test_features.npy")

import torch
import cvtda.classification

cvtda.classification.classify(
    train_images, train_features, train_labels, None,
    test_images, test_features, test_labels, None,
    dump_name = "1/number_visible",
    nn_device = torch.device('cpu'),
    catboost_device = 'CPU', xgboost_device = 'cpu'# , only_get_from_dump = True
)
