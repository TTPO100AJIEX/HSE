import typing

import numpy
import matplotlib.pyplot as plt
import matplotlib.colors as mcolors

import zigzag.topology.metrics as metrics

_CB_PALETTE = [
    "#648FFF",
    "#785EF0",
    "#DC267F",
    "#FE6100",
    "#FFB000",
    "#3CAB20",
    "#6B750C",
    "#A6761D",
    "#D8A21E",
    "#F0E442",
    "#1F77B4",
    "#FF7F0E",
    "#2CA02C",
    "#D62728",
    "#9467BD",
    "#8C564B",
    "#E377C2",
    "#7F7F7F",
    "#BCBD22",
    "#17BECF",
]


def plot_persistence_image(diagram: numpy.ndarray, num_layers: int):
    CMAP = mcolors.LinearSegmentedColormap.from_list("", [(0, "white"), (1, mcolors.to_rgb("#DC267F"))])

    pis = metrics.effective_persistence_image(diagram, num_layers)
    pis_pers = numpy.zeros((num_layers, num_layers))
    for i in range(num_layers):
        for j in range(num_layers):
            if i - j >= 0:
                pis_pers[i - j, j] = pis[i, j]

    fig, ax = plt.subplots(1, 1, figsize=(5, 5))
    im = ax.imshow(numpy.log10(pis_pers), cmap=CMAP, origin="lower")
    ax.set_xlabel("Birth Layer $(\\ell_{\\rm birth})$")
    ax.set_ylabel("Persistence $(\\ell_{\\rm death} - \\ell_{\\rm birth})$")
    fig.colorbar(im, ax=ax).set_label("Log10 number of 1-cycles")
    return fig.tight_layout()


def plot_weighted_inter_layer_persistence(
    diagram: numpy.ndarray,
    num_layers: int,
    alphas: typing.Tuple[float] = (-1.0, 0.0, 0.5, 1, 2)
) -> None:
    fig, ax = plt.subplots(1, 1, figsize=(7, 5))
    for alpha, color in zip(alphas, _CB_PALETTE):
        w_pers = metrics.weighed_inter_layer_persistence(diagram, num_layers, alpha)
        std = numpy.std(w_pers)
        x = numpy.arange(num_layers)
        ax.plot(x, w_pers, color=color, label="$\\alpha=%.2f$" % alpha)
        ax.fill_between(x, w_pers - std, w_pers + std, color=color, alpha=0.2)
    ax.set_xlabel("Layer")
    ax.set_ylabel("Inter-Layer Persistence")
    ax.legend()
    return fig.tight_layout()


def plot_births_relative_frequency(
    diagram: numpy.ndarray,
    num_layers: int,
    alphas: typing.Tuple[float] = (-1.0, 0.0, 0.5, 1, 2)
) -> None:
    fig, ax = plt.subplots(1, 1, figsize=(7, 5))
    for alpha, color in zip(alphas, _CB_PALETTE):
        freq = metrics.births_relative_frequency(diagram, num_layers, alpha)
        std = numpy.std(freq)
        x = numpy.arange(num_layers)
        ax.plot(x, freq, color=color, label="$\\alpha=%.1f$" % alpha)
        ax.fill_between(x, freq - std, freq + std, color=color, alpha=0.2)
    ax.axhline(y=1 / num_layers, label="Uniform Distribution", color="black", linestyle="--", lw=1)
    ax.set_xlabel("Layer", fontsize=17)
    ax.set_ylabel("Births Relative Frequency", fontsize=17)
    ax.legend()
    return fig.tight_layout()
