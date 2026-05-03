import typing
import dataclasses

import torch
import cvtda.logging
import matplotlib.pyplot as plt
from scipy.sparse import csr_matrix

import zigzag.utils
import zigzag.topology


@dataclasses.dataclass(frozen=True)
class Params:
    k_neighbors: int
    dimension: int
    num_layers: typing.Optional[int] = None


def _normalize_params(params: typing.Union[Params, typing.List[Params]]):
    return params if params is list else [params]


def analyze_knn_graphs(knn_graphs: typing.List[csr_matrix], params: Params, dumper: zigzag.utils.UniversalDumper):
    diagrams = dumper.execute(zigzag.topology.compute_zigzag_barcodes, "diagrams", knn_graphs, params.dimension)

    fig, axes = plt.subplots(2, 2, figsize=(13, 7))
    for diagram, ax in zip(diagrams, axes.flat):
        zigzag.topology.plot_persistence_image(diagram, params.num_layers, ax=ax)
    fig.savefig(f"{dumper.directory_}/persistence_image.png")
    fig.savefig(f"{dumper.directory_}/persistence_image.svg")
    plt.close(fig)

    fig, axes = plt.subplots(2, 2, figsize=(13, 7))
    for diagram, ax in zip(diagrams, axes.flat):
        zigzag.topology.plot_weighted_inter_layer_persistence(diagram, params.num_layers, ax=ax)
    fig.savefig(f"{dumper.directory_}/inter_layer_persistence.png")
    fig.savefig(f"{dumper.directory_}/inter_layer_persistence.svg")
    plt.close(fig)

    fig, axes = plt.subplots(2, 2, figsize=(13, 7))
    for diagram, ax in zip(diagrams, axes.flat):
        zigzag.topology.plot_births_relative_frequency(diagram, params.num_layers, ax=ax)
    fig.savefig(f"{dumper.directory_}/births_relative_frequency.png")
    fig.savefig(f"{dumper.directory_}/births_relative_frequency.svg")
    plt.close(fig)


def analyze_vector(
    hidden_states: typing.List[torch.Tensor],
    params: typing.Union[Params, typing.List[Params]],
    dumper: zigzag.utils.UniversalDumper,
):
    cvtda.logging.logger().print("Analyzing as vectors")
    for param in _normalize_params(params):
        subdumper = dumper.make_subdumper(f"{param.k_neighbors}_neighbors")
        knn_graphs = subdumper.execute(
            zigzag.topology.make_knn_graphs_vector, "knn_graphs", hidden_states, param.k_neighbors
        )
        analyze_knn_graphs(knn_graphs, param, subdumper)


def analyze_cubical(
    hidden_states: typing.List[torch.Tensor],
    params: typing.Union[Params, typing.List[Params]],
    dumper: zigzag.utils.UniversalDumper,
):
    cvtda.logging.logger().print("Analyzing as persistence diagrams")
    persistence_diagrams = dumper.execute(
        zigzag.topology.make_cubical_persistence, "persistence_diagrams", hidden_states
    )
    for metric in ["landscape", "persistence_image", "bottleneck"]:
        cvtda.logging.logger().print(f"Trying persistence diagram metric {metric}")
        subdumper = dumper.make_subdumper(metric)
        for param in _normalize_params(params):
            subsubdumper = subdumper.make_subdumper(f"{param.k_neighbors}_neighbors")
            knn_graphs = subsubdumper.execute(
                zigzag.topology.make_knn_graphs_pds, "knn_graphs", persistence_diagrams, params.k_neighbors, metric
            )
            analyze_knn_graphs(knn_graphs, params, subsubdumper)


def analyze_vectorizer(
    hidden_states: typing.List[torch.Tensor],
    params: typing.Union[Params, typing.List[Params]],
    dumper: zigzag.utils.UniversalDumper,
):
    cvtda.logging.logger().print("Analyzing using vectorization")
    features = dumper.execute(
        zigzag.topology.make_features, "features", hidden_states, dump_name=f"{dumper.directory_}/features"
    )
    for param in _normalize_params(params):
        subdumper = dumper.make_subdumper(f"{param.k_neighbors}_neighbors")
        knn_graphs = subdumper.execute(
            zigzag.topology.make_knn_graphs_vector, "knn_graphs", features, params.k_neighbors
        )
        analyze_knn_graphs(knn_graphs, params, subdumper)


def analyze(
    hidden_states: typing.List[torch.Tensor],
    params: typing.Union[Params, typing.List[Params]],
    dumper: zigzag.utils.UniversalDumper,
    class_labels: typing.Optional[torch.Tensor] = None,
):
    analyze_vector(hidden_states, params, dumper.make_subdumper("vectors"))
    if len(hidden_states[0].shape) == 4:
        analyze_vectorizer(hidden_states, params, dumper.make_subdumper("vectorizer"))
        analyze_cubical(hidden_states, params, dumper.make_subdumper("cubical"))

    if class_labels is not None:
        for class_name in torch.unique(class_labels):
            subdumper = dumper.make_subdumper(f"class_{class_name}")
            analyze([hs[class_labels == class_name] for hs in hidden_states], params, subdumper)
