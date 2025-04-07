import os
import sys
sys.path.append(os.path.abspath(os.path.join('../../cvtda')))

sys.path.append(os.path.abspath(os.path.join('../')))

import numpy

train_features = numpy.hstack([
    numpy.load(f"1/{media_code}/train_features.npy")
    for media_code in [ "back", "front", "left", "right" ]
])

test_features = numpy.hstack([
    numpy.load(f"1/{media_code}/test_features.npy")
    for media_code in [ "back", "front", "left", "right" ]
])

print(train_features.shape, test_features.shape)

train_back_labels = numpy.load("1/back/train_labels.npy")
train_front_labels = numpy.load("1/front/train_labels.npy")
train_left_labels = numpy.load("1/left/train_labels.npy")
train_right_labels = numpy.load("1/right/train_labels.npy")
assert (train_back_labels == train_front_labels).all()
assert (train_back_labels == train_left_labels).all()
assert (train_back_labels == train_right_labels).all()
train_labels = train_back_labels
print(train_labels.shape)

test_back_labels = numpy.load("1/back/test_labels.npy")
test_front_labels = numpy.load("1/front/test_labels.npy")
test_left_labels = numpy.load("1/left/test_labels.npy")
test_right_labels = numpy.load("1/right/test_labels.npy")
assert (test_back_labels == test_front_labels).all()
assert (test_back_labels == test_left_labels).all()
assert (test_back_labels == test_right_labels).all()
test_labels = test_back_labels
print(test_labels.shape)


if False:
    import cvtda.utils
    import cvtda.classification

    iv = cvtda.classification.InformationValueFeatureSelector(threshold = 2e-2)
    train_features = iv.fit_transform(train_features, train_labels)
    test_features = iv.transform(test_features)

    print(train_features.shape, test_features.shape)

    df = cvtda.utils.DuplicateFeaturesRemover()
    train_features = df.fit_transform(train_features)
    test_features = df.transform(test_features)

    print(train_features.shape, test_features.shape)


import torch
from utils.classify import classify

classify(
    [
        numpy.load("1/back/train_images.npy"),
        numpy.load("1/front/train_images.npy"),
        numpy.load("1/left/train_images.npy"),
        numpy.load("1/right/train_images.npy")
    ],
    train_features,
    train_labels,
    
    [
        numpy.load("1/back/test_images.npy"),
        numpy.load("1/front/test_images.npy"),
        numpy.load("1/left/test_images.npy"),
        numpy.load("1/right/test_images.npy")
    ],
    test_features,
    test_labels,
    
    nn_device = torch.device('cpu'), dump_name = "1/predictions",
    catboost_device = 'CPU', xgboost_device = 'cpu',
    label_names = [ "no", "other", "yandex", "yango" ], # , only_get_from_dump = True
)
