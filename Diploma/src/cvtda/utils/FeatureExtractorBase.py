import abc
import typing
import inspect
import dataclasses

import numpy
import sklearn.base
import matplotlib.axes
import matplotlib.figure
import matplotlib.pyplot as plt


def make_axes(num_items: int, size: int) -> typing.Tuple[matplotlib.figure.Figure, typing.List[matplotlib.axes.Axes]]:
    num_axes = min(num_items, max(0, 10 - size))
    fig, axes = plt.subplots(1, num_axes, figsize=(size * num_axes, size))
    if not isinstance(axes, numpy.ndarray):
        return fig, [axes]
    return fig, axes


@dataclasses.dataclass
class FeatureExplanation:
    @dataclasses.dataclass
    class PersistenceDiagram:
        diagram: numpy.ndarray
        per_point_stats: numpy.ndarray

        def get_best_points(self):
            non_zero_stats = self.per_point_stats[self.diagram[:, 1] - self.diagram[:, 0] > 0]
            threshold = max(numpy.percentile(non_zero_stats, 75), 1e-8)
            
            best_idx = numpy.argsort(self.per_point_stats)[::-1]
            return best_idx[self.per_point_stats[best_idx] >= threshold]

        def display(self, ax: matplotlib.axes.Axes):
            print(self.diagram)
            print(self.get_best_points())
            def draw(diagram, description: str):
                for dim in range(int(numpy.max(diagram[:, 2], initial=0)) + 1):
                    points = diagram[diagram[:, 2] == dim]
                    if len(points) == 0:
                        continue
                    ax.scatter(points[:, 0], points[:, 1], label=f"{description}: H{dim}")

            limits = [-0.1, self.diagram[:, 1].max() * 1.1]
            draw(self.diagram[self.get_best_points(), :], "Good")
            draw(numpy.delete(self.diagram, self.get_best_points(), axis = 0), "Bad")
            ax.plot(limits, limits, linestyle="dashed", color="black")

            ax.set_xlim(*limits)
            ax.set_ylim(*limits)
            ax.legend(loc="lower right")

    @dataclasses.dataclass
    class Visualization:
        @dataclasses.dataclass
        class Point:
            x: float
            y: float
            label: typing.Optional[str] = None

        image: numpy.ndarray
        title: typing.Optional[str] = None
        mask: typing.Optional[numpy.ndarray] = None
        points: typing.List[Point] = dataclasses.field(default_factory=lambda: [])

        def display(self, ax: matplotlib.axes.Axes):
            ax.imshow(self.image, cmap="gray")
            if self.title is not None:
                ax.set_title(self.title)
            for point in self.points:
                ax.scatter(point.x, point.y, label=point.label)
            if self.mask is not None:
                ax.imshow(self.mask, cmap="gray", alpha=0.75)
            ax.legend()
            ax.axis("off")

    persistence_diagrams: typing.List[PersistenceDiagram] = dataclasses.field(default_factory=lambda: [])
    messages: typing.List[str] = dataclasses.field(default_factory=lambda: [])
    visualizations: typing.List[Visualization] = dataclasses.field(default_factory=lambda: [])

    def extend(self, other):
        self.persistence_diagrams.extend(other.persistence_diagrams)
        self.messages.extend(other.messages)
        self.visualizations.extend(other.visualizations)

    def display(self, feature_name: str):
        print(f"Explaining {feature_name}:")
        for message in self.messages:
            print(f"    {message}")

        matplotlib.rcParams.update({"font.size": 6})

        if len(self.persistence_diagrams) != 0:
            fig, axes = make_axes(len(self.persistence_diagrams), 3)
            fig.suptitle(feature_name)
            for ax, diagram_explanation in zip(axes, self.persistence_diagrams):
                diagram_explanation.display(ax)
            fig.tight_layout()

        if len(self.visualizations) != 0:
            fig, axes = make_axes(len(self.visualizations), 1.5)
            fig.suptitle(feature_name)
            for ax, visualization in zip(axes, self.visualizations):
                visualization.display(ax)
            fig.tight_layout()


class FeatureExtractorBase(sklearn.base.TransformerMixin, abc.ABC):
    """
    Base feature extractor class.

    Attributes
    ----------
    PRESETS : ``Presets``
        Settings presets of the feature extractor.
    """

    @dataclasses.dataclass(frozen=True)
    class Presets:
        """
        Settings presets container of the feature extractor.

        Attributes
        ----------
        full : ``object``
            The full, slow pipeline.
        reduced : ``object``
            The reduced pipeline with good balance between speed and quality.
        quick : ``object``
            The quick pipeline.
        """

        full: object
        reduced: object
        quick: object

    PRESETS: Presets = None

    def __init_subclass__(cls, **kwargs):
        super().__init_subclass__(**kwargs)
        if "settings" not in inspect.signature(cls.__init__).parameters.keys():
            return
        if cls.PRESETS is None:
            raise TypeError(f"{cls.__name__} must define PRESETS")

    def nest_feature_names(self, prefix: str, names: typing.List[str]) -> typing.List[str]:
        return [f"{prefix} -> {name}" for name in names]

    def unnest_feature_name(self, name: str) -> typing.Tuple[str, str]:
        idx = name.index(" -> ")
        return name[:idx], name[idx + 4 :]

    @abc.abstractmethod
    def feature_names(self) -> typing.List[str]:
        """
        Gives a list of features extracted by this class.

        Returns
        -------
        ``list[str]``
            Feature names.
        """
        pass

    @abc.abstractmethod
    def explain(self, feature_name: str, input: numpy.ndarray) -> FeatureExplanation:
        pass
