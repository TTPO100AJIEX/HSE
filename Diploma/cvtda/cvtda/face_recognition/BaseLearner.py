import abc
import typing

import numpy
import joblib
import itertools
import matplotlib.pyplot as plt

import cvtda.logging
import cvtda.neural_network


class BaseLearner:
    def __init__(self, n_jobs: int = -1):
        self.n_jobs_ = n_jobs

    
    @abc.abstractmethod
    def fit(self, train: cvtda.neural_network.Dataset, val: typing.Optional[cvtda.neural_network.Dataset]):
        pass

            
    def estimate_quality(
        self,
        dataset: cvtda.neural_network.Dataset,
        ax: typing.Optional[plt.Axes] = None
    ):
        def calculate_distance_(i: int, j: int):
            return (i, j, self.calculate_distance_(i, j, dataset))

        idxs = list(itertools.product(range(len(dataset)), range(len(dataset))))
        distances_flat = joblib.Parallel(n_jobs = 1)(
            joblib.delayed(calculate_distance_)(i, j)
            for i, j in cvtda.logging.logger().pbar(idxs, desc = "Calculating pairwise distances")
        )

        correct_dists, incorrect_dists = [ ], [ ]
        for i, j, distance in cvtda.logging.logger().pbar(distances_flat, desc = "Analyzing distances"):
            label1, label2 = dataset.get_labels([ i, j ])
            if label1 == label2: correct_dists.append(distance)
            else: incorrect_dists.append(distance)
        
        ax.set_ylim(-1, 2)
        ax.plot(correct_dists, numpy.zeros_like(correct_dists), 'x')
        ax.plot(incorrect_dists, numpy.ones_like(incorrect_dists), 'x')


    @abc.abstractmethod
    def calculate_distance_(self, first: int, second: int, dataset: cvtda.neural_network.Dataset):
        pass