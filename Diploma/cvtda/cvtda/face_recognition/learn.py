import os
import typing

import numpy
import torch
import matplotlib.pyplot as plt

import cvtda.dumping
import cvtda.neural_network
from .BaseLearner import BaseLearner
from .DiagramsLearner import DiagramsLearner
from .NNLearner import NNLearner
from .SimpleTopologicalLearner import SimpleTopologicalLearner

def learn(
    train_images: numpy.ndarray,
    train_features: numpy.ndarray,
    train_labels: numpy.ndarray,
    train_diagrams: typing.List[numpy.ndarray],

    test_images: numpy.ndarray,
    test_features: numpy.ndarray,
    test_labels: numpy.ndarray,
    test_diagrams: typing.List[numpy.ndarray],

    n_jobs: int = -1,
    random_state: int = 42,
    dump_name: typing.Optional[str] = None,

    nn_device: torch.device = torch.device('cuda'),
    nn_batch_size: int = 64,
    nn_learning_rate: float = 1e-4,
    nn_epochs: int = 25,
    nn_margin: int = 1,
    nn_latent_dim: int = 256,
):
    nn_train = cvtda.neural_network.Dataset(
        train_images, train_diagrams, train_features, train_labels, n_jobs = n_jobs, device = nn_device
    )
    nn_test = cvtda.neural_network.Dataset(
        test_images, test_diagrams, test_features, test_labels, n_jobs = n_jobs, device = nn_device
    )

    def classify_one(learner: BaseLearner, name: str, ax: plt.Axes):
        print(f'Trying {name} - {learner}')
        learner.fit(nn_train, nn_test)
        ax.set_title(name)
        learner.estimate_quality(nn_test, ax)

    nn_kwargs = dict(
        n_jobs = n_jobs,
        random_state = random_state,
        device = nn_device,
        batch_size = nn_batch_size,
        learning_rate = nn_learning_rate,
        n_epochs = nn_epochs,
        margin = nn_margin,
        latent_dim = nn_latent_dim
    )
    classifiers = [
        SimpleTopologicalLearner(n_jobs = n_jobs),
        DiagramsLearner(n_jobs = n_jobs),
        NNLearner(**nn_kwargs, skip_diagrams = True, skip_images = False, skip_features = True),
        NNLearner(**nn_kwargs, skip_diagrams = True, skip_images = True, skip_features = False),
        NNLearner(**nn_kwargs, skip_diagrams = True, skip_images = False, skip_features = False),
        # NNLearner(**nn_kwargs, skip_diagrams = False, skip_images = True, skip_features = True)
    ]

    names = [
        'SimpleTopologicalLearner',
        'DiagramsLearner',
        'NNClassifier_images',
        'NNClassifier_features',
        'NNClassifier_features_images',
        'NNClassifier_diagrams'
    ]

    figure, axes = plt.subplots(2, 3, figsize = (20, 7))
    for args in zip(classifiers, names, axes.flat):
        classify_one(*args)

    dumper = cvtda.dumping.dumper()
    if (dump_name is not None) and isinstance(dumper, cvtda.dumping.NumpyDumper):
        file = dumper.get_file_name_(cvtda.dumping.dump_name_concat(dump_name, "distributions"))
        os.makedirs(os.path.dirname(file), exist_ok = True)
        figure.savefig(file[:-4] + ".svg")
        figure.savefig(file[:-4] + ".png")
