import os
import sys
sys.path.append(os.path.abspath(os.path.join('../../cvtda')))

import numpy
import torchvision

train = torchvision.datasets.CIFAR10('cifar-10', train = True, download = False)
test = torchvision.datasets.CIFAR10('cifar-10', train = False, download = False)

train_images = numpy.array([ numpy.array(item[0]) / 255 for item in train ])
train_labels = numpy.array([ item[1] for item in train ])

test_images = numpy.array([ numpy.array(item[0]) / 255 for item in test ])
test_labels = numpy.array([ item[1] for item in test ])

print(train_images.shape, test_images.shape)

train_features = numpy.load("20/train_features.npy")
test_features = numpy.load("20/test_features.npy")

print(train_features.shape, test_features.shape)

import cvtda.classification

cvtda.classification.classify(
    train_images, train_features, train_labels, None,
    test_images, test_features, test_labels, None,
    label_names = train.classes, dump_name = "22/predictions",
    catboost_device = 'CPU'
)
