import typing

import torch
import gudhi.hera

import cvtda.neural_network
from .BaseLearner import BaseLearner


class DiagramsLearner(BaseLearner):
    def fit(self, train: cvtda.neural_network.Dataset, val: typing.Optional[cvtda.neural_network.Dataset]):
        pass

    def calculate_distance_(self, first: int, second: int, dataset: cvtda.neural_network.Dataset):
        _, _, *diagrams1 = dataset.get_feature(first, skip_diagrams = False)
        _, _, *diagrams2 = dataset.get_feature(second, skip_diagrams = False)
        assert len(diagrams1) == len(diagrams2)

        distance_vector = []
        for i in range(len(diagrams1), 2):
            diag1 = diagrams1[i].cpu().numpy()
            diag2 = diagrams2[i].cpu().numpy()

            diag1 = diag1[diag1[:, 0] < diag1[:, 1]]
            diag2 = diag2[diag2[:, 0] < diag2[:, 1]]

            distance_vector.append(gudhi.hera.bottleneck_distance(diag1, diag2))
            
        return torch.sqrt(torch.sum(distance_vector ** 2))
