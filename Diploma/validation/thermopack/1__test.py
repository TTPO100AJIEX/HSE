import os
import sys
sys.path.append(os.path.abspath(os.path.join('../../cvtda')))

import tqdm
import numpy
import torchvision
import torchvision.transforms.v2

transforms = torchvision.transforms.v2.Compose([
    torchvision.transforms.v2.Resize((48, 48), antialias = True)
])

train = torchvision.datasets.ImageFolder('data/train', transform = transforms)
test = torchvision.datasets.ImageFolder('data/test', transform = transforms)

# train_images = numpy.array([ numpy.array(item[0]) / 255 for item in tqdm.tqdm(train) ])[:40000]
# train_labels = numpy.array([ item[1] for item in tqdm.tqdm(train) ])[:40000]

# test_images = numpy.array([ numpy.array(item[0]) / 255 for item in tqdm.tqdm(test) ])
# test_labels = numpy.array([ item[1] for item in tqdm.tqdm(test) ])

# numpy.save("train_images.npy", train_images)
# numpy.save("train_labels.npy", train_labels)

# numpy.save("test_images.npy", test_images)
# numpy.save("test_labels.npy", test_labels)

# import cvtda.topology
# extractor = cvtda.topology.FeatureExtractor(only_get_from_dump = True, return_diagrams = False)
# train_features = extractor.fit_transform(train_images, "1/train")
# test_features = extractor.transform(test_images, "1/test")

# numpy.save("train_features.npy", train_features)
# numpy.save("test_features.npy", test_features)

train_images = numpy.load("train_images.npy")
train_labels = numpy.load("train_labels.npy")

test_images = numpy.load("test_images.npy")
test_labels = numpy.load("test_labels.npy")

train_features = numpy.load("train_features.npy")
test_features = numpy.load("test_features.npy")

import torch
import cvtda.classification

cvtda.classification.classify(
    train_images, train_features, train_labels, None,
    test_images, test_features, test_labels, None,
    dump_name = "1/predictions",
    nn_device = torch.device('cpu'),
    catboost_device = 'CPU', xgboost_device = 'cpu'# , only_get_from_dump = True
)
