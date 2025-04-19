import os
import sys
sys.path.append(os.path.abspath(os.path.join('../../src')))

import tqdm
import numpy
import torchvision
import torchvision.transforms.v2

transforms = torchvision.transforms.v2.Compose([
    torchvision.transforms.v2.Resize((64, 64), antialias = True)
])

train = torchvision.datasets.ImageFolder('data/train', transform = transforms)
test = torchvision.datasets.ImageFolder('data/test', transform = transforms)

train_images = numpy.array([ numpy.array(item[0]) / 255 for item in tqdm.tqdm(train) ])
train_labels = numpy.array([ item[1] for item in tqdm.tqdm(train) ])

test_images = numpy.array([ numpy.array(item[0]) / 255 for item in tqdm.tqdm(test) ])
test_labels = numpy.array([ item[1] for item in tqdm.tqdm(test) ])

print(train_images.shape, test_images.shape)

import cvtda.topology
extractor = cvtda.topology.FeatureExtractor(only_get_from_dump = False, return_diagrams = False)
extractor = extractor.fit(train_images, "1/train")
train_features = extractor.transform(train_images, "1/train")
test_features = extractor.transform(test_images, "1/test")
