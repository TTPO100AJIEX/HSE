import typing

import numpy
import gtda.images
import sklearn.base

from cvtda.utils import set_random_seed
from cvtda.logging import DevNullLogger, CLILogger

class GreyscaleToFiltrations(sklearn.base.TransformerMixin):
    def __init__(
        self,
        n_jobs: int = -1,
        verbose: bool = True,
        random_state: int = 42,

        binarizer_threshold: float = 0.1,
        
        height_filtration_directions: typing.Iterable[typing.Tuple[float, float]] = [
            [ -1, -1 ], [ 1, 1 ], [ 1, -1 ], [ -1, 1 ],
            [ 0, -1 ], [ 0, 1 ], [ -1, 0 ], [ 1, 0 ]
        ],
        radial_filtration_centers: typing.Iterable[typing.Tuple[int, int]] = [ ],
        density_filtration_radiuses: typing.Iterable[int] = [ 1, 3 ]
    ):
        self.fitted_ = False
        self.n_jobs_ = n_jobs
        self.random_state_ = random_state
        set_random_seed(self.random_state_)
        self.logger_ = CLILogger() if verbose else DevNullLogger()

        if len(radial_filtration_centers) == 0:
            self.logger_.print("Warning: radial_filtration_centers is empty")

        self.binarizer_ = gtda.images.Binarizer(threshold = binarizer_threshold, n_jobs = n_jobs)

        self.filtrations_ = [
            *[
                gtda.images.HeightFiltration(direction = numpy.array(direction), n_jobs = n_jobs)
                for direction in height_filtration_directions
            ],
            *[
                gtda.images.RadialFiltration(center = numpy.array(center), n_jobs = n_jobs)
                for center in radial_filtration_centers
            ],
            gtda.images.DilationFiltration(n_jobs = n_jobs),
            gtda.images.ErosionFiltration(n_jobs = n_jobs),
            gtda.images.SignedDistanceFiltration(n_jobs = n_jobs),
            *[
                gtda.images.DensityFiltration(radius = radius, n_jobs = n_jobs)
                for radius in density_filtration_radiuses
            ]
        ]

    def fit(self, images: numpy.ndarray):
        set_random_seed(self.random_state_)
        
        self.logger_.print('Fitting the binarizer')
        images_bin = self.binarizer_.fit_transform(images)

        for filtration in self.logger_.loop(self.filtrations_, desc = "Fitting the filtrations"):
            filtration.fit(images_bin)

        self.logger_.print('Fitting complete')
        self.fitted_ = True
        return self
    
    def transform(self, images: numpy.ndarray) -> numpy.ndarray:
        assert self.fitted_ is True, 'fit() must be called before transform()'
        set_random_seed(self.random_state_)
        
        images_bin = self.binarizer_.transform(images)

        return numpy.stack([
            images_bin,
            *[
                filtration.transform(images_bin)
                for filtration in self.logger_.loop(self.filtrations_, desc = "Filtrations")
            ]
        ], axis = 1)
