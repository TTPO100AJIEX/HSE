import os
import sys
sys.path.append(os.path.abspath(os.path.join('../../cvtda')))

import numpy
import torchvision
import torchvision.transforms.v2

transform = torchvision.transforms.v2.Compose([
    torchvision.transforms.v2.Resize((128, 128), antialias = True)
])

train = torchvision.datasets.Imagenette('imagenette', split = 'train', size = '160px', transform = transform, download = False)
test = torchvision.datasets.Imagenette('imagenette', split = 'val', size = '160px', transform = transform, download = False)

train_images = numpy.array([ numpy.array(item[0]) / 255 for item in train ])
train_labels = numpy.array([ item[1] for item in train ])

test_images = numpy.array([ numpy.array(item[0]) / 255 for item in test ])
test_labels = numpy.array([ item[1] for item in test ])

print(train_images.shape, test_images.shape)

import cvtda.topology
extractor = cvtda.topology.FeatureExtractor(only_get_from_dump = False, return_diagrams = False)
extractor = extractor.fit(train_images, "1/train")
train_features = extractor.transform(train_images, "1/train")
test_features = extractor.transform(test_images, "1/test")
