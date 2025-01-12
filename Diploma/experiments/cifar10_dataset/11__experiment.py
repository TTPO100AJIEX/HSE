import os
import sys
import itertools
sys.path.append(os.path.abspath(os.path.join('../../cvtda')))

import typing

import numpy
import gtda.images
import torchvision
import skimage.color

import cvtda.utils
import cvtda.topology

train_ds = torchvision.datasets.CIFAR10('cifar-10', train = True, download = False)
test_ds = torchvision.datasets.CIFAR10('cifar-10', train = False, download = False)

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

def make_features(channel: int, split_idx: typing.Optional[int], inverter, binarizer, filtration) -> typing.Tuple[numpy.ndarray, numpy.ndarray]:
    print(f'channel = {channel}, split_idx = {split_idx}, inverter = {inverter}, binarizer = {binarizer}, filtration = {filtration}')

    train = numpy.array([ make_image(item[0], channel, split_idx) for item in train_ds ])
    test = numpy.array([ make_image(item[0], channel, split_idx) for item in test_ds ])
    print(f'Datasets: {train.shape}, {test.shape}')
    
    if inverter is not None:
        train = inverter.fit_transform(train)
        test = inverter.transform(test)

    if binarizer is not None:
        train = binarizer.fit_transform(train)
        test = binarizer.transform(test)
        
    if filtration is not None:
        train = filtration.fit_transform(train)
        test = filtration.transform(test)
    
    filtrations_to_diagrams = cvtda.topology.FiltrationsToDiagrams(verbose = False)
    train = filtrations_to_diagrams.fit_transform(train)
    test = filtrations_to_diagrams.transform(test)
    print(f'Diagrams: {train.shape}, {test.shape}')

    if len(train[0]) < 96:
        n_bins = 32
    elif len(train[0]) < 192:
        n_bins = 64
    else:
        n_bins = 128
    print(f"Bins: {n_bins}")

    digrams_to_features = cvtda.topology.DiagramsToFeatures(batch_size = 850, n_bins = n_bins, verbose = False)
    train = digrams_to_features.fit_transform(train)
    test = digrams_to_features.transform(test)
    print(f'Features: {train.shape}, {test.shape}')
    return train, test

def process(inverted: bool, channel: int, split_idx: typing.Optional[int]) -> typing.Tuple[numpy.ndarray, numpy.ndarray]:
    dir = f"11/inv_{channel}/{split_idx}" if inverted else f"11/{channel}/{split_idx}"
    if os.path.exists(f"{dir}/test.npy"):
        return
    
    train_features, test_features = [ ], [ ]

    train, test = make_features(
        channel,
        split_idx,
        inverter = (gtda.images.Inverter(n_jobs = -1) if inverted else None),
        binarizer = None,
        filtration = None
    )
    train_features.append(train)
    test_features.append(test)
    del train
    del test
    
    if split_idx is None:
        centers = [ 5, 12, 18, 25 ]
    else:
        centers = [ 5, 10 ]

    greyscale_to_filtrations = cvtda.topology.GreyscaleToFiltrations(
        radial_filtration_centers = list(itertools.product(centers, centers))
    )
    for filtration in greyscale_to_filtrations.filtrations_:
        train, test = make_features(
            channel,
            split_idx,
            inverter = (gtda.images.Inverter(n_jobs = -1) if inverted else None),
            binarizer = gtda.images.Binarizer(threshold = 0.4),
            filtration = filtration
        )
        train_features.append(train)
        test_features.append(test)
    
    os.makedirs(dir, exist_ok = True)
    numpy.save(f"{dir}/train.npy", numpy.hstack(train_features))
    numpy.save(f"{dir}/test.npy", numpy.hstack(test_features))

for channel in [ 'red', 'green', 'blue', 'gray', 'saturation', 'value' ]:
    print(f'>>> Calculating channel {channel}')
    process(False, channel, None)
    for split in range(9):
        process(False, channel, split)
        
for channel in [ 'red', 'green', 'blue', 'gray', 'saturation', 'value' ]:
    print(f'>>> Calculating channel {channel} for an inverted image')
    process(True, channel, None)
    for split in range(9):
        process(True, channel, split)
