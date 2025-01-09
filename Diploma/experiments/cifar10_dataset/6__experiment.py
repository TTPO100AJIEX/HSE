import os
import sys
import itertools
sys.path.append(os.path.abspath(os.path.join('../../cvtda')))

import tqdm
import numpy
import torch
import gtda.images
import torchvision
import torchvision.transforms.v2

import cvtda.utils
import cvtda.topology

transform = torchvision.transforms.v2.Compose([
    torchvision.transforms.v2.ToImage(),
    torchvision.transforms.v2.ToDtype(torch.float32, scale = True)
])

train_ds = torchvision.datasets.CIFAR10('cifar-10', transform = transform, train = True, download = False)
test_ds = torchvision.datasets.CIFAR10('cifar-10', transform = transform, train = False, download = False)

def make_features(name: str, binarizer, filtration):
    if os.path.exists(f"6/{name}/test_features.npy"):
        return

    train = numpy.array([ numpy.array(item[0]) for item in train_ds ])
    test = numpy.array([ numpy.array(item[0]) for item in test_ds ])
    
    if binarizer is not None:
        train = binarizer.fit_transform(train)
        test = binarizer.transform(test)
        
    if filtration is not None:
        train = filtration.fit_transform(train)
        test = filtration.transform(test)

    filtrations_to_diagrams = cvtda.topology.FiltrationsToDiagrams(homology_dimensions = [ 0, 1, 2 ])
    train = filtrations_to_diagrams.fit_transform(train)
    test = filtrations_to_diagrams.transform(test)
    print(train.shape, test.shape)

    n_bins = (64 if len(train) < 512 else 128)
    digrams_to_features = cvtda.topology.DiagramsToFeatures(batch_size = 625, n_bins = n_bins)
    train = digrams_to_features.fit_transform(train)
    test = digrams_to_features.transform(test)
    print(train.shape, test.shape)

    ok_features = []
    for idx in tqdm.trange(train.shape[1]):
        if numpy.std(train[:, idx]) > 1e-6:
            ok_features.append(idx)
    train = train[:, ok_features]
    test = test[:, ok_features]
    print(train.shape, test.shape)

    duplicates_remover = cvtda.utils.DuplicateFeaturesRemover()
    train = duplicates_remover.fit_transform(train)
    test = duplicates_remover.transform(test)
    print(train.shape, test.shape)

    os.makedirs(f"6/{name}", exist_ok = True)
    numpy.save(f"6/{name}/train_features.npy", train)
    numpy.save(f"6/{name}/test_features.npy", test)


make_features("raw", binarizer = None, filtration = None)

greyscale_to_filtrations = cvtda.topology.GreyscaleToFiltrations(
    binarizer_threshold = 0.4,
    height_filtration_directions = list(itertools.product([ 0, 1, -1 ], [ 0, 1, -1 ], [ 0, 1, -1 ]))[1:],
    radial_filtration_centers = list(itertools.product([ 0, 1, 2 ], [ 3, 8, 13, 18, 23, 28 ], [ 3, 8, 13, 18, 23, 28 ]))
)
for i, filtration in enumerate(greyscale_to_filtrations.filtrations_):
    print(f"{i}/{len(greyscale_to_filtrations.filtrations_)}). {filtration}")
    make_features(
        f"{type(filtration).__name__}{i}",
        binarizer = gtda.images.Binarizer(threshold = 0.4),
        filtration = filtration
    )
