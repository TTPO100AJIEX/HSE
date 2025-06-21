import os
import sys
import itertools
sys.path.append(os.path.abspath(os.path.join('../../src')))

import typing

import tqdm
import numpy
import joblib
import gtda.images
import torchvision
import skimage.color

import cvtda.utils
import cvtda.topology

def make_image(image, channel: int, split_idx: typing.Optional[int]) -> numpy.ndarray:
    image = numpy.array(image)
    match channel:
        case 'red':
            image = image[:, :, 0]
        case 'green':
            image = image[:, :, 1]
        case 'blue':
            image = image[:, :, 2]
        case 'gray':
            image = skimage.color.rgb2gray(image)
        case 'saturation':
            image = skimage.color.rgb2hsv(image)[:, :, 1]
        case 'value':
            image = skimage.color.rgb2hsv(image)[:, :, 2]
        case _:
            raise NotImplementedError
    assert image.shape == (32, 32)

    if split_idx is None:
        return image
    
    for start_x in range(0, 17, 8):
        for start_y in range(0, 17, 8):
            if split_idx == 0:
                return image[start_x:start_x + 16, start_y:start_y + 16]
            else:
                split_idx -= 1

def make_features(
    channel: int,
    split_idx: typing.Optional[int],
    inverter,
    binarizer,
    filtration,
    n_jobs: int = 1
) -> typing.Tuple[numpy.ndarray, numpy.ndarray]:
    train = numpy.array(
        joblib.Parallel(n_jobs = n_jobs)(
            joblib.delayed(make_image)(item[0], channel, split_idx)
            for item in torchvision.datasets.CIFAR10('cifar-10', train = True, download = False)
        )
    )
    test = numpy.array(
        joblib.Parallel(n_jobs = n_jobs)(
            joblib.delayed(make_image)(item[0], channel, split_idx)
            for item in torchvision.datasets.CIFAR10('cifar-10', train = False, download = False)
        )
    )

    if inverter is not None:
        train = inverter.fit_transform(train)
        test = inverter.transform(test)

    if binarizer is not None:
        train = binarizer.fit_transform(train)
        test = binarizer.transform(test)
        
    if filtration is not None:
        train = filtration.fit_transform(train)
        test = filtration.transform(test)
    
    filtrations_to_diagrams = cvtda.topology.FiltrationsToDiagrams(verbose = False, n_jobs = n_jobs)
    train = filtrations_to_diagrams.fit_transform(train)
    test = filtrations_to_diagrams.transform(test)

    if len(train[0]) < 96:
        n_bins = 32
    elif len(train[0]) < 192:
        n_bins = 64
    else:
        n_bins = 128

    digrams_to_features = cvtda.topology.DiagramsToFeatures(batch_size = 500, n_bins = n_bins, verbose = False, n_jobs = n_jobs)
    train = digrams_to_features.fit_transform(train)
    test = digrams_to_features.transform(test)
    return train, test

def process(inverted: bool, channel: int, split_idx: typing.Optional[int], binarizer_threshold: float) -> typing.Tuple[numpy.ndarray, numpy.ndarray]:
    dir_suffix = f"{channel}/{split_idx}"
    dir_prefix = f"E:/{int(binarizer_threshold * 10)}"
    dir = f"{dir_prefix}/inv_{dir_suffix}" if inverted else f"{dir_prefix}/{dir_suffix}"
    if os.path.exists(f"{dir}/test.npy"):
        return
    
    if split_idx is None:
        centers = [ 5, 12, 18, 25 ]
    else:
        centers = [ 5, 10 ]

    greyscale_to_filtrations = cvtda.topology.GreyscaleToFiltrations(
        n_jobs = 1,
        radial_filtration_centers = list(itertools.product(centers, centers))
    )
    features = joblib.Parallel(return_as = 'generator', n_jobs = -1)(
        joblib.delayed(make_features)(
            channel,
            split_idx,
            inverter = (gtda.images.Inverter(n_jobs = 1) if inverted else None),
            binarizer = gtda.images.Binarizer(threshold = binarizer_threshold, n_jobs = 1),
            filtration = filtration,
            n_jobs = 1
        )
        for filtration in greyscale_to_filtrations.filtrations_
    )

    train_features, test_features = [ ], [ ]

    desc = f'channel = {channel}, split_idx = {split_idx}, inverted = {inverted}, binarizer_threshold = {binarizer_threshold}'
    for train, test in tqdm.tqdm(features, desc = desc, total = len(greyscale_to_filtrations.filtrations_)):
        train_features.append(train)
        test_features.append(test)

    train, test = make_features(
        channel,
        split_idx,
        inverter = (gtda.images.Inverter(n_jobs = -1) if inverted else None),
        binarizer = None,
        filtration = None,
        n_jobs = -1
    )
    train_features.append(train)
    test_features.append(test)
    
    train_features = numpy.hstack(train_features)
    test_features = numpy.hstack(test_features)

    os.makedirs(dir, exist_ok = True)
    numpy.save(f"{dir}/train.npy", train_features)
    numpy.save(f"{dir}/test.npy", test_features)

for binarizer_threshold_mul in range(1, 9):
    binarizer_threshold = binarizer_threshold_mul / 10

    for channel in [ 'red', 'green', 'blue', 'gray', 'saturation', 'value' ]:
        print(f'>>> Calculating channel {channel} with binarizer_threshold = {binarizer_threshold}')
        process(False, channel, None, binarizer_threshold)
        for split in range(9):
            process(False, channel, split, binarizer_threshold)
            
    for channel in [ 'red', 'green', 'blue', 'gray', 'saturation', 'value' ]:
        print(f'>>> Calculating channel {channel} for an inverted image with binarizer_threshold = {binarizer_threshold}')
        process(True, channel, None, binarizer_threshold)
        for split in range(9):
            process(True, channel, split, binarizer_threshold)
