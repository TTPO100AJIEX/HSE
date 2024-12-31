import typing

import numpy
import gtda.images
import sklearn.base

class GreyscaleImageToFiltration(sklearn.base.TransformerMixin):
    def __init__(
        self,
        n_jobs: int = -1,
        verbose: bool = True,
        random_state: int = 42,
        
        height_filtration_directions: typing.List[typing.Tuple[float, float]] = [
            [ -1, -1 ], [ 1, 1 ], [ 1, -1 ], [ -1, 1 ],
            [ 0, -1 ], [ 0, 1 ], [ -1, 0 ], [ 1, 0 ]
        ],

        radial_filtration_num_centers: int = 5,
        radial_filtration_metrics: typing.List[str] = [ "chebyshev", "euclidean", "manhattan"  ]
        
    ):
        pass

    def fit(self, images: numpy.ndarray):
        pass

    def transform(self, images: numpy.ndarray):
        pass




BINARIZATION_THESHOLD = 0.2

height_filtration_directions = [
    [ -1, -1 ], [ 1, 1 ], [ 1, -1 ], [ -1, 1 ],
    [ 0, -1 ], [ 0, 1 ], [ -1, 0 ], [ 1, 0 ]
]


radial_filtration_centers = list(itertools.product([ 7, 14, 21 ], [ 7, 14, 21 ]))
radial_filtration_metrics = [ "chebyshev", "euclidean", "manhattan"  ]

density_filtration_metrics = [ "euclidean" , "manhattan", "cosine" ]
density_filtration_radiuses = [ 1, 5, 15 ]

FILTRATIONS = [
    *[ [ gtda.images.HeightFiltration, { 'direction': numpy.array(direction), 'n_jobs': -1 } ] for direction in height_filtration_directions ],
    *[
        [ gtda.images.RadialFiltration, { 'center': numpy.array(center), 'metric': metric, 'n_jobs': -1 } ]
        for center in radial_filtration_centers
        for metric in radial_filtration_metrics
    ],
    [ gtda.images.DilationFiltration, { 'n_jobs': -1 } ],
    [ gtda.images.ErosionFiltration, { 'n_jobs': -1 } ],
    [ gtda.images.SignedDistanceFiltration, { 'n_jobs': -1 } ],
    *[
        [ gtda.images.DensityFiltration, { 'radius': radius, 'metric': metric, 'n_jobs': -1 } ]
        for metric in density_filtration_metrics
        for radius in density_filtration_radiuses
    ]
]

def make_filtrations(images: numpy.ndarray):
    images_bin = gtda.images.Binarizer(threshold = BINARIZATION_THESHOLD).fit_transform(images)
    filtrations = [
        filtration[0](**filtration[1]).fit_transform(images_bin)
        for filtration in tqdm.tqdm(FILTRATIONS, desc = 'filtrations')
    ]
    return [ images, images_bin ] + filtrations
