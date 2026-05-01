import typing

import numpy
import torch
import cvtda.logging
import gtda.homology
import gtda.diagrams
import sklearn.neighbors
from scipy.sparse import csr_matrix


def make_knn_graphs_vector(hidden_states: typing.List[torch.Tensor], k_neighbors: int) -> typing.List[csr_matrix]:
    def impl(hs: torch.Tensor):
        hs = hs.flatten(start_dim=1).numpy()
        return sklearn.neighbors.kneighbors_graph(hs, n_neighbors=k_neighbors, n_jobs=-1)

    return [impl(hs) for hs in cvtda.logging.logger().pbar(hidden_states, desc="KNN graphs")]


def make_knn_graphs_pds(
    persistence: typing.List[numpy.ndarray], k_neighbors: int, metric: str
) -> typing.List[csr_matrix]:
    def impl(diagrams: numpy.ndarray):
        return sklearn.neighbors.kneighbors_graph(
            gtda.diagrams.PairwiseDistance(metric=metric, n_jobs=-1).fit_transform(diagrams),
            n_neighbors=k_neighbors,
            metric="precomputed",
            n_jobs=-1,
        )

    return [impl(diagrams) for diagrams in cvtda.logging.logger().pbar(persistence, desc="KNN graphs")]


def make_cubical_persistence(hidden_states: typing.List[torch.Tensor]) -> typing.List[numpy.ndarray]:
    def impl(hs: torch.Tensor):
        hs = numpy.linalg.norm(hs.numpy(), axis=1)
        return gtda.homology.CubicalPersistence(n_jobs=-1).fit_transform(hs)

    return [impl(hs) for hs in cvtda.logging.logger().pbar(hidden_states, desc="Cubical persistence")]
