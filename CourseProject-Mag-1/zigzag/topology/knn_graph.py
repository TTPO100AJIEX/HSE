import typing

import numpy
import torch
import sklearn.neighbors


def make_knn_graph(hidden_states: torch.Tensor, k_neighbors: int) -> numpy.ndarray:
    if len(hidden_states.shape) == 2:
        data = hidden_states.detach().cpu().numpy()
        return sklearn.neighbors.kneighbors_graph(data, n_neighbors=k_neighbors).toarray()
    else:
        assert False, f"Unsupported shape of hidden_states: {len(hidden_states.shape)}"


def make_knn_graphs(hidden_states: typing.List[torch.Tensor], k_neighbors: int) -> typing.List[numpy.ndarray]:
    return [make_knn_graph(hs, k_neighbors) for hs in hidden_states]
