import typing

import numpy
import sklearn.base
import gtda.homology

from cvtda.utils import set_random_seed
from cvtda.logging import DevNullLogger, CLILogger

class PointCloudsToDiagrams(sklearn.base.TransformerMixin):
    def __init__(
        self,
        n_jobs: int = -1,
        verbose: bool = True,
        random_state: int = 42,

        metric: str = "euclidean",
        homology_dimensions: typing.List[int] = [0, 1, 2],
        coeff: int = 2
    ):
        self.fitted_ = False
        self.n_jobs_ = n_jobs
        self.random_state_ = random_state
        set_random_seed(self.random_state_)
        self.logger_ = CLILogger() if verbose else DevNullLogger()

        self.persistence_ = gtda.homology.VietorisRipsPersistence(
            metric = metric,
            homology_dimensions = homology_dimensions,
            coeff = coeff,
            n_jobs = n_jobs
        )


    def fit(self, point_clouds: typing.List[numpy.ndarray]):
        set_random_seed(self.random_state_)
        
        self.logger_.print('Fitting the persistence')
        self.persistence_.fit(point_clouds)

        self.logger_.print('Fitting complete')
        self.fitted_ = True
        return self
    
    def transform(self, point_clouds: typing.List[numpy.ndarray]) -> numpy.ndarray:
        assert self.fitted_ is True, 'fit() must be called before transform()'
        set_random_seed(self.random_state_)
        
        self.logger_.print('Calculating the persistence')
        return self.persistence_.transform(point_clouds)
