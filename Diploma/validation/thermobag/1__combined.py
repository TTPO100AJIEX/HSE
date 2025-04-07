import os
import sys
sys.path.append(os.path.abspath(os.path.join('../../cvtda')))

import numpy

train_labels = numpy.load("train_labels_combined.npy")
test_labels = numpy.load("test_labels_combined.npy")

train_features = numpy.load("train_features.npy")
test_features = numpy.load("test_features.npy")

import torch
import cvtda.classification

cvtda.classification.classify(
    None, train_features, train_labels, None,
    None, test_features, test_labels, None,
    dump_name = "1/combined",
    nn_device = torch.device('cpu'),
    catboost_device = 'CPU', xgboost_device = 'cpu'# , only_get_from_dump = True
)
