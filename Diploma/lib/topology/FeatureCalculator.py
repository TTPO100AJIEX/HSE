import os
import random
import typing

import tqdm
import numpy
import gtda.curves
import gtda.diagrams

def determine_filtering_epsilon(diagrams: numpy.ndarray, percentile: int):
    life = (diagrams[:, :, 1] - diagrams[:, :, 0]).flatten()
    return numpy.percentile(life[life != 0], percentile)

def apply_filtering(diagrams: numpy.ndarray, eps: float):
    filtering = gtda.diagrams.Filtering(epsilon = eps)
    return filtering.fit_transform(diagrams)


def set_random_seed(seed: int):
    random.seed(seed)
    numpy.random.seed(seed)
    os.environ["PYTHONHASHSEED"] = str(seed)


AMPLITUDE_METRICS = [
    { "id": "bottleneck", "metric": "bottleneck", "metric_params": { } },

    { "id": "wasserstein-1", "metric": "wasserstein", "metric_params": { "p": 1 } },
    { "id": "wasserstein-2", "metric": "wasserstein", "metric_params": { "p": 2 } },

    { "id": "betti-1", "metric": "betti", "metric_params": { "p": 1, 'n_bins': -1 } },
    { "id": "betti-2", "metric": "betti", "metric_params": { "p": 2, 'n_bins': -1 } },
    
    { "id": "landscape-1-1", "metric": "landscape", "metric_params": { "p": 1, "n_layers": 1, 'n_bins': -1 } },
    { "id": "landscape-1-2", "metric": "landscape", "metric_params": { "p": 1, "n_layers": 2, 'n_bins': -1 } },
    { "id": "landscape-2-1", "metric": "landscape", "metric_params": { "p": 2, "n_layers": 1, 'n_bins': -1 } },
    { "id": "landscape-2-2", "metric": "landscape", "metric_params": { "p": 2, "n_layers": 2, 'n_bins': -1 } },

    { "id": "silhouette-1-1", "metric": "silhouette", "metric_params": { "p": 1, "power": 1, 'n_bins': -1 } },
    { "id": "silhouette-1-2", "metric": "silhouette", "metric_params": { "p": 1, "power": 2, 'n_bins': -1 } },
    { "id": "silhouette-2-1", "metric": "silhouette", "metric_params": { "p": 2, "power": 1, 'n_bins': -1 } },
    { "id": "silhouette-2-2", "metric": "silhouette", "metric_params": { "p": 2, "power": 2, 'n_bins': -1 } },

    { "id": "heat-1-1.6", "metric": "heat", "metric_params": { "p": 1, "sigma": 1.6, 'n_bins': -1 } },
    { "id": "heat-1-3.2", "metric": "heat", "metric_params": { "p": 1, "sigma": 3.2, 'n_bins': -1 } },
    { "id": "heat-2-1.6", "metric": "heat", "metric_params": { "p": 2, "sigma": 1.6, 'n_bins': -1 } },
    { "id": "heat-2-3.2", "metric": "heat", "metric_params": { "p": 2, "sigma": 3.2, 'n_bins': -1 } },

    { "id": "persistence_image-1-1.6", "metric": "persistence_image", "metric_params": { "p": 1, "sigma": 1.6, 'n_bins': -1 } },
    { "id": "persistence_image-1-3.2", "metric": "persistence_image", "metric_params": { "p": 1, "sigma": 3.2, 'n_bins': -1 } },
    { "id": "persistence_image-2-1.6", "metric": "persistence_image", "metric_params": { "p": 2, "sigma": 1.6, 'n_bins': -1 } },
    { "id": "persistence_image-2-3.2", "metric": "persistence_image", "metric_params": { "p": 2, "sigma": 3.2, 'n_bins': -1 } }
]

class FeatureCalculator:
    def __init__(
        self,
        n_jobs: int = -1,
        verbose: bool = True,
        random_state: int = 42,
        filtering_percentile: int = 10
    ):
        self.n_jobs = n_jobs
        self.verbose = verbose
        self.random_state = random_state
        self.filtering_percentile = filtering_percentile


    def calc_stats_bulk(self, data: numpy.ndarray) -> numpy.ndarray:
        return numpy.ma.hstack([
            numpy.ma.max(data, axis = 1, keepdims = True),
            numpy.ma.mean(data, axis = 1, keepdims = True),
            numpy.ma.std(data, axis = 1, keepdims = True),
            numpy.ma.sum(data, axis = 1, keepdims = True),
            numpy.ma.median(data, axis = 1, keepdims = True),
            numpy.ma.sqrt(numpy.ma.sum(data ** 2, axis = 1, keepdims = True))
        ]).filled(0)

    def calc_batch_stats(self, data: numpy.ndarray, homology_dimensions: typing.Iterable[int]) -> numpy.ndarray:
        return numpy.hstack([ self.calc_stats_bulk(data[:, d, :]) for d in homology_dimensions ])
    

    def calc_betti_features(self, diagrams: numpy.ndarray) -> numpy.ndarray:
        if self.verbose:
            print('Calculating Betti features')

        betti_curve = gtda.diagrams.BettiCurve(n_bins = 100, n_jobs = self.n_jobs)
        betti_curves = betti_curve.fit_transform(diagrams)

        betti_derivative = gtda.curves.Derivative()
        betti_curves = betti_derivative.fit_transform(betti_curves)
        
        return self.calc_batch_stats(betti_curves, betti_curve.homology_dimensions_)
        
    def calc_landscape_features(self, diagrams: numpy.ndarray) -> numpy.ndarray:
        if self.verbose:
            print('Calculating landscape features')
        persistence_landscape = gtda.diagrams.PersistenceLandscape(n_layers = 1, n_bins = 100, n_jobs = self.n_jobs)
        landscape = persistence_landscape.fit_transform(diagrams)
        return self.calc_batch_stats(landscape, persistence_landscape.homology_dimensions_)

    def calc_silhouette_features(self, diagrams: numpy.ndarray, powers: typing.Union[int, typing.List[int]] = [ 1, 2 ]) -> numpy.ndarray:
        if isinstance(powers, int):
            silhouette = gtda.diagrams.Silhouette(power = powers, n_bins = 100, n_jobs = self.n_jobs)
            silhouettes = silhouette.fit_transform(diagrams)
            return self.calc_batch_stats(silhouettes, silhouette.homology_dimensions_)
        else:
            if self.verbose:
                print('Calculating silhouette features')
            return numpy.hstack([ self.calc_silhouette_features(diagrams, power) for power in powers ])

    def calc_entropy_features(self, diagrams: numpy.ndarray) -> numpy.ndarray:
        if self.verbose:
            print('Calculating entropy features')
        entropy = gtda.diagrams.PersistenceEntropy(normalize = True, nan_fill_value = 0, n_jobs = self.n_jobs)
        return entropy.fit_transform(diagrams)
    
    def calc_number_of_points_features(self, diagrams: numpy.ndarray) -> numpy.ndarray:
        if self.verbose:
            print('Calculating number of points features')
        number_of_points = gtda.diagrams.NumberOfPoints(n_jobs = self.n_jobs)
        return number_of_points.fit_transform(diagrams)
    
    def calc_amplitude_features(self, diagrams: numpy.ndarray, **metric) -> numpy.ndarray:
        if len(metric) == 0:
            metrics = tqdm.tqdm(AMPLITUDE_METRICS, desc = 'amplitudes') if self.verbose else AMPLITUDE_METRICS
            return numpy.hstack([ self.calc_amplitude_features(diagrams, **metric) for metric in metrics ])
        
        metric_params = metric['metric_params'].copy()
        if metric_params.get('n_bins', None) == -1:
            metric_params['n_bins'] = 100

        amplitude = gtda.diagrams.Amplitude(metric = metric['metric'], metric_params = metric_params, n_jobs = self.n_jobs)
        features = amplitude.fit_transform(diagrams)
        return numpy.hstack([
            features,
            numpy.linalg.norm(features, axis = 1, ord = 1).reshape(-1, 1),
            numpy.linalg.norm(features, axis = 1, ord = 2).reshape(-1, 1),
        ])

    def calc_lifetime_features(self, diagrams: numpy.ndarray, eps: float = 0.0) -> numpy.ndarray:
        if self.verbose:
            print('Calculating lifetime features')

        birth, death, dim = diagrams[:, :, 0], diagrams[:, :, 1], diagrams[:, :, 2]
        bd2 = (birth + death) / 2.0
        life = death - birth
        
        mask = (life < eps)
        bd2 = numpy.ma.array(bd2, mask = mask)
        life = numpy.ma.array(life, mask = mask)

        bd2_features = [ self.calc_stats_bulk(bd2) ]
        life_features = [ self.calc_stats_bulk(life) ]
        for d in range(0, int(numpy.max(dim)) + 1):
            mask = (dim != d)
            dim_bd2 = numpy.ma.array(bd2, mask = mask)
            dim_life = numpy.ma.array(life, mask = mask)
            bd2_features.append(self.calc_stats_bulk(dim_bd2))
            life_features.append(self.calc_stats_bulk(dim_life))

        return numpy.hstack([ *life_features, *bd2_features ])

    def calc_features(self, diagrams: numpy.ndarray) -> numpy.ndarray:
        set_random_seed(self.random_state)
        
        eps = determine_filtering_epsilon(diagrams, self.filtering_percentile)
        diagrams = apply_filtering(diagrams, eps)
        if self.verbose:
            print('Filtered diagrams:', diagrams.shape)
        return numpy.hstack([
           self.calc_betti_features           (diagrams     ),
           self.calc_landscape_features       (diagrams     ),
           self.calc_silhouette_features      (diagrams     ),
           self.calc_entropy_features         (diagrams     ),
           self.calc_number_of_points_features(diagrams     ),
           self.calc_amplitude_features       (diagrams     ),
           self.calc_lifetime_features        (diagrams, eps)
        ])
