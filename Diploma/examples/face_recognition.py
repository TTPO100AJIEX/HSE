import os
import sys
sys.path.append(os.path.abspath(os.path.join('../src')))

import numpy
import torchvision
import torchvision.transforms.v2

transform = torchvision.transforms.v2.Compose([
    torchvision.transforms.v2.CenterCrop((128, 128)),
    torchvision.transforms.v2.Resize((64, 64))
])

train = torchvision.datasets.ImageFolder('../experiments/labeled_faces_in_the_wild/lfw/training')
train_images = numpy.array([ item[0] for item in train ])[:20] / 255
train_labels = numpy.array([ item[1] for item in train ])[:20]

test = torchvision.datasets.ImageFolder('../experiments/labeled_faces_in_the_wild/lfw/testing')
test_images = numpy.array([ item[0] for item in test ])[:10] / 255
test_labels = numpy.array([ item[1] for item in test ])[:10]



import cvtda.topology
extractor = cvtda.topology.FeatureExtractor()
train_features = extractor.fit_transform(train_images, "train")
test_features = extractor.transform(test_images, "test")

extractor = cvtda.topology.FeatureExtractor(
    return_diagrams = True,
    only_get_from_dump = True
)
train_diagrams = extractor.fit_transform(train_images, "train")
test_diagrams = extractor.transform(test_images, "test")

import cvtda.face_recognition
cvtda.face_recognition.learn(
    train_images, train_features, train_labels, train_diagrams,
    test_images, test_features, test_labels, test_diagrams
)
