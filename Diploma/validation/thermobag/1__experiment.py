import os
import sys
sys.path.append(os.path.abspath(os.path.join('../../cvtda')))

import tqdm
import numpy
import pandas
import torchvision
import torchvision.transforms.v2

transforms = torchvision.transforms.v2.Compose([
    torchvision.transforms.v2.Resize((48, 48), antialias = True)
])

train = torchvision.datasets.ImageFolder('thermobag/train', transform = transforms)
test = torchvision.datasets.ImageFolder('thermobag/test', transform = transforms)

passes = pandas.read_csv("passes.csv")
labels_map = { row["pass_id"]: int(row["presented"]) for _, row in passes.iterrows() }

train_images = numpy.array([ numpy.array(item[0]) / 255 for item in tqdm.tqdm(train) ])
train_labels = numpy.array([
    labels_map[item[0][len("thermobag/train/train/"):-len("_thermobag.jpg")]]
    for item in tqdm.tqdm(train.samples)
])

test_images = numpy.array([ numpy.array(item[0]) / 255 for item in tqdm.tqdm(test) ])
test_labels = numpy.array([
    labels_map[item[0][len("thermobag/test/test/"):-len("_thermobag.jpg")]]
    for item in tqdm.tqdm(test.samples)
])

print(train_images.shape, test_images.shape)

import cvtda.topology
extractor = cvtda.topology.FeatureExtractor(only_get_from_dump = False, return_diagrams = False)
extractor = extractor.fit(train_images, "1/train")
train_features = extractor.transform(train_images, "1/train")
test_features = extractor.transform(test_images, "1/test")
