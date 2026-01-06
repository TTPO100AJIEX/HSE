import math
import typing

import numpy
import joblib
import itertools
import gtda.images
import gtda.homology

import cvtda.utils
import cvtda.logging

from . import utils
import cvtda.dumping
from .interface import TopologicalExtractor


class FiltrationExtractor(TopologicalExtractor):
    def __init__(
        self,
        filtration_class,
        filtation_kwargs: dict,
        binarizer_threshold: float,

        n_jobs: int = -1,
        reduced: bool = True,
        only_get_from_dump: bool = False,
        return_diagrams: bool = False,
        **kwargs
    ):
        super().__init__(
            supports_rgb = False,
            n_jobs = n_jobs,
            reduced = reduced,
            only_get_from_dump = only_get_from_dump,
            return_diagrams = return_diagrams,
            filtration_class = filtration_class,
            filtation_kwargs = filtation_kwargs,
            binarizer_threshold = binarizer_threshold,
            **kwargs
        )

        self.binarizer_ = gtda.images.Binarizer(threshold = binarizer_threshold, n_jobs = self.n_jobs_)
        self.filtration_ = filtration_class(**filtation_kwargs, n_jobs = self.n_jobs_)
        self.persistence_ = None


    def get_diagrams_(self, images: numpy.ndarray, do_fit: bool, dump_name: typing.Optional[str] = None):
        cvtda.logging.logger().print(f"FiltrationExtractor: processing {dump_name}, do_fit = {do_fit}, filtration = {self.filtration_}")
        
        if do_fit and (self.persistence_ is None):
            dims = list(range(len(images.shape) - 1))
            self.persistence_ = gtda.homology.CubicalPersistence(homology_dimensions = dims, n_jobs = self.n_jobs_)
        
        bin_images = utils.process_iter(self.binarizer_, images, do_fit)
        assert bin_images.shape == images.shape

        filtrations = utils.process_iter(self.filtration_, bin_images, do_fit)
        assert filtrations.shape == images.shape

        return utils.process_iter_dump(self.persistence_, filtrations, do_fit, self.diagrams_dump_(dump_name))


class FiltrationsExtractor(cvtda.utils.FeatureExtractorBase):
    def __init__(
        self,

        n_jobs: int = -1,
        reduced: bool = True,
        only_get_from_dump: bool = False,
        return_diagrams: bool = False,

        binarizer_thresholds: typing.Optional[typing.List[float]] = None,
        height_filtration_directions: typing.Optional[typing.Iterable[typing.Tuple[float, float]]] = None,
        num_radial_filtrations: int = 4,
        density_filtration_radiuses: typing.Iterable[int] = [ 1, 3 ],
    ):
        if not binarizer_thresholds:
            if reduced:
                binarizer_thresholds = [ 0.2, 0.4, 0.6 ]
            else:
                binarizer_thresholds = [ 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9 ]

        self.fitted_ = False
        self.n_jobs_ = n_jobs
        self.reduced_ = reduced
        self.feature_names_ = []
        self.return_diagrams_ = return_diagrams
        self.filtrations_kwargs_ = {
            'n_jobs': 1,
            'reduced': reduced,
            'only_get_from_dump': only_get_from_dump,
            'return_diagrams': return_diagrams
        }

        self.binarizer_thresholds_ = binarizer_thresholds
        self.height_filtration_directions_ = height_filtration_directions
        self.num_radial_filtrations_ = num_radial_filtrations
        self.density_filtration_radiuses_ = density_filtration_radiuses

        self.filtration_extractors_: typing.List[typing.Tuple[FiltrationExtractor, str]] = []

    def feature_names(self) -> typing.List[str]:
        feature_names = []
        for extractor, _, readable_name in self.filtration_extractors_:
            feature_names.extend(self.nest_feature_names(readable_name, extractor.feature_names()))
        return feature_names

    def fit(self, images: numpy.ndarray, dump_name: typing.Optional[str] = None):
        self.fit_transform(images, dump_name)
        return self
    
    def transform(self, images: numpy.ndarray, dump_name: typing.Optional[str] = None) -> numpy.ndarray:
        assert self.fitted_ is True, 'fit() must be called before transform()'
        cvtda.logging.logger().print("Applying filtrations")
        return self.do_work_(images, do_fit = False, dump_name = dump_name)
    
    def fit_transform(self, images: numpy.ndarray, dump_name: typing.Optional[str] = None) -> numpy.ndarray:
        assert len(images.shape) >= 3, f'{len(images.shape) - 1}d images are not supported'
        cvtda.logging.logger().print("Fitting filtrations")
        
        shape = images.shape
        if (len(shape) == 4) and (shape[-1] == 3):
            shape = shape[:-1]

        if self.height_filtration_directions_ is None:
            coords = len(shape) - 1
            directions = list(itertools.product(*([ [ -1, 0, 1 ] ] * coords)))
            directions = filter(lambda item: not all(i == 0 for i in item), directions)
            self.height_filtration_directions_ = list(directions)

        self._fill_filtrations(*shape[1:])

        result = self.do_work_(images, do_fit = True, dump_name = dump_name)
        self.fitted_ = True
        return result

    def do_work_(self, images: numpy.ndarray, do_fit: bool, dump_name: typing.Optional[str] = None) -> numpy.ndarray:
        def do_work_one(extractor: FiltrationExtractor, name, readable_name):
            extractor_dump_name = cvtda.dumping.dump_name_concat(dump_name, name)
            with cvtda.logging.DevNullLogger():
                result = utils.process_iter(extractor, images, do_fit, dump_name = extractor_dump_name)
                return ((extractor, name, readable_name), result) if do_fit else result

        parallel = cvtda.utils.parallel(do_work_one, self.filtration_extractors_, return_as = 'generator', n_jobs = self.outer_n_jobs_)
        outputs = list(cvtda.logging.logger().pbar(parallel, total = len(self.filtration_extractors_)))
        if do_fit:
            self.filtration_extractors_ = [ output[0] for output in outputs ]
            outputs = [ output[1] for output in outputs ]
        result = utils.hstack(outputs, not self.return_diagrams_)
        if not self.return_diagrams_:
            assert result.shape == (len(images), len(self.feature_names()))
        return result


    def _fill_filtrations(self, *shape: typing.List[int]):
        self._do_fill_filtrations(*shape)
        n_jobs = joblib.effective_n_jobs(self.n_jobs_)
        self.outer_n_jobs_ = math.gcd(n_jobs, len(self.filtration_extractors_))
        self.filtrations_kwargs_['n_jobs'] = n_jobs // self.outer_n_jobs_
        self.filtration_extractors_ = []
        self._do_fill_filtrations(*shape)
    
    def _do_fill_filtrations(self, *shape: typing.List[int]):
        self.filtration_extractors_ = [ ]
        for binarizer_threshold in self.binarizer_thresholds_:
            self._add_height_filtrations(binarizer_threshold)
            self._add_radial_filtrations(binarizer_threshold, *shape)
            self._add_dilation_filtrations(binarizer_threshold)
            self._add_erosion_filtrations(binarizer_threshold)
            self._add_signed_distance_filtrations(binarizer_threshold)
            self._add_density_filtrations(binarizer_threshold)

    def _add_height_filtrations(self, binarizer_threshold: float):
        for direction in self.height_filtration_directions_:
            name = f'{int(binarizer_threshold * 10)}/HeightFiltrartion_{direction[0]}_{direction[1]}'
            readable_name = f'HeightFiltration with d = ({direction[0]}, {direction[1]}), bin. thr. = 0.{int(binarizer_threshold * 10)}'
            extractor = FiltrationExtractor(
                gtda.images.HeightFiltration, { 'direction': numpy.array(direction) }, binarizer_threshold, **self.filtrations_kwargs_
            )
            self.filtration_extractors_.append((extractor, name, readable_name))
            
    def _add_radial_filtrations(self, binarizer_threshold: float, *shape: typing.List[int]):
        points = [ cvtda.utils.spread_points(coord, self.num_radial_filtrations_) for coord in shape ]
        for center in list(itertools.product(*points)):
            name = f'{int(binarizer_threshold * 10)}/RadialFiltration_{center[0]}_{center[1]}'
            readable_name = f'RadialFiltration with c = ({center[0]}, {center[1]}), bin. thr. = 0.{int(binarizer_threshold * 10)}'
            extractor = FiltrationExtractor(
                gtda.images.RadialFiltration, { 'center': numpy.array(center) }, binarizer_threshold, **self.filtrations_kwargs_
            )
            self.filtration_extractors_.append((extractor, name, readable_name))

    def _add_dilation_filtrations(self, binarizer_threshold: float):
        if self.reduced_:
            return
        name = f'{int(binarizer_threshold * 10)}/DilationFiltration'
        readable_name = f'DilationFiltration, bin. thr. = 0.{int(binarizer_threshold * 10)}'
        extractor = FiltrationExtractor(gtda.images.DilationFiltration, { }, binarizer_threshold, **self.filtrations_kwargs_)
        self.filtration_extractors_.append((extractor, name, readable_name))

    def _add_erosion_filtrations(self, binarizer_threshold: float):
        if self.reduced_:
            return
        name = f'{int(binarizer_threshold * 10)}/ErosionFiltration'
        readable_name = f'ErosionFiltration, bin. thr. = 0.{int(binarizer_threshold * 10)}'
        extractor = FiltrationExtractor(gtda.images.ErosionFiltration, { }, binarizer_threshold, **self.filtrations_kwargs_)
        self.filtration_extractors_.append((extractor, name, readable_name))

    def _add_signed_distance_filtrations(self, binarizer_threshold: float):
        if self.reduced_:
            return
        name = f'{int(binarizer_threshold * 10)}/SignedDistanceFiltration'
        readable_name = f'SignedDistanceFiltration, bin. thr. = 0.{int(binarizer_threshold * 10)}'
        extractor = FiltrationExtractor(gtda.images.SignedDistanceFiltration, { }, binarizer_threshold, **self.filtrations_kwargs_)
        self.filtration_extractors_.append((extractor, name, readable_name))
        
    def _add_density_filtrations(self, binarizer_threshold: float):
        if self.reduced_:
            return
        for radius in self.density_filtration_radiuses_:
            name = f'{int(binarizer_threshold * 10)}/DensityFiltration_{radius}'
            readable_name = f'DensityFiltration with r = {radius}, bin. thr. = 0.{int(binarizer_threshold * 10)}'
            extractor = FiltrationExtractor(gtda.images.DensityFiltration, { 'radius': radius }, binarizer_threshold, **self.filtrations_kwargs_)
            self.filtration_extractors_.append((extractor, name, readable_name))
