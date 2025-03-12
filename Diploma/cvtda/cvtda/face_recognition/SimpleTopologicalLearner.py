import typing

import torch

import cvtda.neural_network
from .BaseLearner import BaseLearner


class SimpleTopologicalLearner(BaseLearner):
    def fit(self, train: cvtda.neural_network.Dataset, val: typing.Optional[cvtda.neural_network.Dataset]):
        pass

    def calculate_distance_(self, first: int, second: int, dataset: cvtda.neural_network.Dataset):
        _, embedding1 = dataset.get_feature(first, skip_diagrams = True)
        _, embedding2 = dataset.get_feature(second, skip_diagrams = True)
        return torch.sqrt(torch.sum((embedding1 - embedding2) ** 2))
