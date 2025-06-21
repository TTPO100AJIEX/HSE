import os
import sys
sys.path.append(os.path.abspath(os.path.join('../../src')))

import numpy
import torchvision
import torchvision.transforms.v2

transform = torchvision.transforms.v2.Compose([
    torchvision.transforms.v2.Resize((64, 64), antialias = True)
])

train = torchvision.datasets.CIFAR10('cifar-10', train = True, download = False, transform = transform)
test = torchvision.datasets.CIFAR10('cifar-10', train = False, download = False, transform = transform)

train_images = numpy.array([ numpy.array(item[0]) / 255 for item in train ])
train_labels = numpy.array([ item[1] for item in train ])

test_images = numpy.array([ numpy.array(item[0]) / 255 for item in test ])
test_labels = numpy.array([ item[1] for item in test ])

mean = train_images.mean(axis = (0, 1, 2))
std = train_images.std(axis = (0, 1, 2))
train_images = (train_images - mean) / std
test_images = (test_images - mean) / std

print(train_images.shape, test_images.shape)
print(train_images.mean(axis = (0, 1, 2)), train_images.std(axis = (0, 1, 2)))
print(test_images.mean(axis = (0, 1, 2)), test_images.std(axis = (0, 1, 2)))

import cvtda.topology
extractor = cvtda.topology.FeatureExtractor(
    only_get_from_dump = False,
    return_diagrams = False,
    num_radial_filtrations = 8,
    binarizer_thresholds = [ 0.2, 0.3, 0.4, 0.5, 0.6, 0.7 ]
)
extractor = extractor.fit(train_images, "21/train")
train_features = extractor.transform(train_images, "21/train")
test_features = extractor.transform(test_images, "21/test")
