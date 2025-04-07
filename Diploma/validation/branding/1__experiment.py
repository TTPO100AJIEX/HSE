import os
import sys
sys.path.append(os.path.abspath(os.path.join('../../cvtda')))

import tqdm
import numpy
import torchvision
import torchvision.transforms.v2

import cvtda.topology

for media_code in [ "back", "front", "left", "right" ]:
    print(media_code)
    if os.path.exists(f"1/{media_code}/test_features.npy"):
        continue

    if os.path.exists(f"1/{media_code}/test_labels.npy"):
        train_images = numpy.load(f"1/{media_code}/train_images.npy")
        train_labels = numpy.load(f"1/{media_code}/train_labels.npy")
        test_images = numpy.load(f"1/{media_code}/test_images.npy")
        test_labels = numpy.load(f"1/{media_code}/test_labels.npy")
    else:
        transforms = torchvision.transforms.v2.Compose([
            torchvision.transforms.v2.Resize((48, 48), antialias = True)
        ])

        train = torchvision.datasets.ImageFolder(f'data/train/{media_code}', transform = transforms)
        test = torchvision.datasets.ImageFolder(f'data/test/{media_code}', transform = transforms)
        print(train.classes, test.classes)

        train_images = numpy.array([ numpy.array(item[0]) / 255 for item in tqdm.tqdm(train) ])
        train_labels = numpy.array([ item[1] for item in tqdm.tqdm(train) ])

        test_images = numpy.array([ numpy.array(item[0]) / 255 for item in tqdm.tqdm(test) ])
        test_labels = numpy.array([ item[1] for item in tqdm.tqdm(test) ])

        os.makedirs(f"1/{media_code}")
        numpy.save(f"1/{media_code}/train_images.npy", train_images)
        numpy.save(f"1/{media_code}/train_labels.npy", train_labels)
        numpy.save(f"1/{media_code}/test_images.npy", test_images)
        numpy.save(f"1/{media_code}/test_labels.npy", test_labels)

    print(train_images.shape, test_images.shape)

    extractor = cvtda.topology.FeatureExtractor(only_get_from_dump = False, return_diagrams = False)
    extractor = extractor.fit(train_images, f"1/{media_code}/train")
    train_features = extractor.transform(train_images, f"1/{media_code}/train")
    test_features = extractor.transform(test_images, f"1/{media_code}/test")
    
    numpy.save(f"1/{media_code}/train_features.npy", train_features)
    numpy.save(f"1/{media_code}/test_features.npy", test_features)
