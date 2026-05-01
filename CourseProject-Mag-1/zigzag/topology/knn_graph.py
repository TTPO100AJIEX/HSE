import typing
import warnings

import numpy
import torch
import scipy.sparse
import sklearn.neighbors

import cvtda.logging


def make_knn_graph(hidden_states: torch.Tensor, k_neighbors: int) -> scipy.sparse.csr_matrix:
    if len(hidden_states.shape) == 2:
        data = hidden_states.detach().cpu().numpy()
        with warnings.catch_warnings():
            warnings.filterwarnings("ignore", message=".*libiomp.*")
            return sklearn.neighbors.kneighbors_graph(data, n_neighbors=k_neighbors, n_jobs=-1)
    else:
        assert False, f"Unsupported shape of hidden_states: {len(hidden_states.shape)}"


def make_knn_graphs(
    hidden_states: typing.List[torch.Tensor], k_neighbors: int
) -> typing.List[scipy.sparse.csr_matrix]:
    return [make_knn_graph(hs, k_neighbors) for hs in cvtda.logging.logger().pbar(hidden_states, desc="KNN graphs")]
