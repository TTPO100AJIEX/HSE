import sys
sys.path.append('../src')

import warnings
warnings.filterwarnings("ignore")

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--quick", action = argparse.BooleanOptionalAction, default = False)
parser.add_argument("--clean", action = argparse.BooleanOptionalAction, default = False)
args = parser.parse_args()

if args.quick:
    FRACTION = {
        'mnist': 0.05
    }

if args.quick:
    features_extraction_params = dict(
        reduced = True,
        with_inverted = False,
        num_radial_filtrations = 2,
        binarizer_thresholds = [0.5],
        height_filtration_directions = [(-1, -1), (1, -1), (-1, 1), (1, 1)]
    )
else:
    features_extraction_params = dict()

if args.quick:
    classification_params = dict(
        catboost_iterations = 100, nn_epochs = 4, nn_batch_size = 64, nn_learning_rate = 1e-4
    )
else:
    classification_params = dict()

import shutil
import numpy
import torchvision
import cvtda.utils
import cvtda.topology
import cvtda.logging
import cvtda.classification
import sklearn.model_selection

def subset(images: numpy.ndarray, labels: numpy.ndarray, fraction: float):
    idxs, _ = sklearn.model_selection.train_test_split(
        numpy.arange(len(labels)),
        stratify = labels,
        train_size = fraction,
        random_state = 42
    )
    return images[idxs], labels[idxs]

def classification(name, train, test):
    print(f"Processing {name}")
    folder = f"{name}/results"
    if args.clean:
        shutil.rmtree(folder, ignore_errors = True)

    train_images = numpy.array([ numpy.array(item[0]) / 255 for item in train ])
    train_labels = numpy.array([ item[1] for item in train ])

    test_images = numpy.array([ numpy.array(item[0]) / 255 for item in test ])
    test_labels = numpy.array([ item[1] for item in test ])

    if args.quick:
        train_images, train_labels = subset(train_images, train_labels, FRACTION[name])
        test_images, test_labels = subset(test_images, test_labels, FRACTION[name])

        if len(train_images.shape) == 4:
            train_images = cvtda.utils.rgb2gray(train_images)
            test_images = cvtda.utils.rgb2gray(test_images)

    from cvtda.topology.GeometryExtractor import GrayGeometryExtractor
    features_extractor = GrayGeometryExtractor()
    features_extractor.fit_transform(train_images)

    return
    
    features_extractor = cvtda.topology.FeatureExtractor(
        only_get_from_dump = False, return_diagrams = False, **features_extraction_params
    )
    train_features = features_extractor.fit_transform(train_images, f"{folder}/train")
    test_features = features_extractor.transform(test_images, f"{folder}/test")

    return
    
    diagrams_extractor = cvtda.topology.FeatureExtractor(
        only_get_from_dump = True, return_diagrams = True, **features_extraction_params
    )
    train_diagrams = diagrams_extractor.fit_transform(train_images, f"{folder}/train")
    test_diagrams = diagrams_extractor.transform(test_images, f"{folder}/test")

    results = cvtda.classification.classify(
        train_images, train_features, train_labels, train_diagrams,
        test_images, test_features, test_labels, test_diagrams,
        dump_name = f"{folder}/predictions", only_get_from_dump = False,
        label_names = train.classes, **classification_params
    )
    print(results)

import time
start = time.time()
with cvtda.logging.CLILogger():
    classification(
        'mnist',
        torchvision.datasets.MNIST('mnist', train = True, download = True),
        torchvision.datasets.MNIST('mnist', train = False, download = True)
    )
print(time.time() - start)