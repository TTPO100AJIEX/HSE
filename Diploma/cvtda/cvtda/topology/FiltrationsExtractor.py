import typing

import numpy
import itertools
import gtda.images
import sklearn.base
import gtda.homology

import cvtda.utils
import cvtda.logging

from .Pipeline import Pipeline
from .DiagramVectorizer import DiagramVectorizer


class FiltrationExtractor(Pipeline):
    def __init__(
        self,
        filtration_class,
        filtation_kwargs: dict,
        binarizer_threshold: float,
        n_jobs: int = -1,
        reduced: bool = True
    ):
        super().__init__(
            n_jobs = n_jobs,
            reduced = reduced,
            filtration_class = filtration_class,
            filtation_kwargs = filtation_kwargs,
            binarizer_threshold = binarizer_threshold
        )

        self.binarizer_ = gtda.images.Binarizer(threshold = binarizer_threshold, n_jobs = self.n_jobs_)
        self.filtration_ = filtration_class(**filtation_kwargs, n_jobs = self.n_jobs_)

        self.persistence_ = gtda.homology.CubicalPersistence(homology_dimensions = [ 0, 1 ], n_jobs = self.n_jobs_)
        self.vectorizer_ = DiagramVectorizer(n_jobs = self.n_jobs_, reduced = self.reduced_)


    def process_rgb_(self, rgb_images: numpy.ndarray, do_fit: bool, dump_name: typing.Optional[str] = None) -> numpy.ndarray:
        return numpy.zeros((len(rgb_images), 0))
    
    def process_gray_(self, gray_images: numpy.ndarray, do_fit: bool, dump_name: typing.Optional[str] = None) -> numpy.ndarray:
        cvtda.logging.logger().print(f"FiltrationExtractor: processing {dump_name}, do_fit = {do_fit}, filtration = {self.filtration_}")
        bin_images = self.process_iter_(self.binarizer_, gray_images, do_fit)
        assert bin_images.shape == gray_images.shape

        filtrations = self.process_iter_(self.filtration_, bin_images, do_fit)
        assert filtrations.shape == gray_images.shape

        diagrams = self.process_iter_dump_(
            self.persistence_,
            filtrations,
            do_fit,
            self.dump_name_concat_(dump_name, "diagrams")
        )
        return self.process_iter_dump_(
            self.vectorizer_,
            diagrams,
            do_fit,
            self.dump_name_concat_(dump_name, "features")
        )


class FiltrationsExtractor(sklearn.base.TransformerMixin):
    def __init__(
        self,
        binarizer_thresholds: typing.List[float] = [ 0.2, 0.4, 0.6, 0.8 ],
        height_filtration_directions: typing.Iterable[typing.Tuple[float, float]] = [
            [ -1, -1 ], [ 1, 1 ], [ 1, -1 ], [ -1, 1 ],
            [ 0, -1 ], [ 0, 1 ], [ -1, 0 ], [ 1, 0 ]
        ],
        num_radial_filtrations: int = 4,
        density_filtration_radiuses: typing.Iterable[int] = [ 1, 3 ],
        n_jobs: int = -1,
        reduced: bool = True
    ):
        self.fitted_ = False
        self.n_jobs_ = n_jobs
        self.reduced_ = reduced

        self.binarizer_thresholds_ = binarizer_thresholds
        self.height_filtration_directions_ = height_filtration_directions
        self.num_radial_filtrations_ = num_radial_filtrations
        self.density_filtration_radiuses_ = density_filtration_radiuses


    def fit(self, images: numpy.ndarray, dump_name: typing.Optional[str] = None):
        assert len(images.shape) >= 3, f'{len(images.shape) - 1}d images are not supported'
        self._fill_filtrations(images.shape[1], images.shape[2])
        for filtration_extractor, name in cvtda.logging.logger().pbar(self.filtration_extractors_, desc = "Fitting filtrations"):
            with cvtda.logging.DevNullLogger():
                filtration_extractor.fit(images, self.dump_name_concat_(dump_name, name))
        self.fitted_ = True
        return self
    
    def transform(self, images: numpy.ndarray, dump_name: typing.Optional[str] = None) -> numpy.ndarray:
        assert self.fitted_ is True, 'fit() must be called before transform()'
        
        features = [ ]
        for filtration_extractor, name in cvtda.logging.logger().pbar(self.filtration_extractors_, desc = "Applying filtrations"):
            with cvtda.logging.DevNullLogger():
                features.append(filtration_extractor.transform(images, self.dump_name_concat_(dump_name, name)))
        return numpy.hstack(features)
    
    def fit_transform(self, images: numpy.ndarray, dump_name: typing.Optional[str] = None) -> numpy.ndarray:
        return self.fit(images, dump_name = dump_name).transform(images, dump_name = dump_name)


    def _fill_filtrations(self, width: int, height: int):
        radial_x = cvtda.utils.spread_points(width, self.num_radial_filtrations_)
        radial_y = cvtda.utils.spread_points(height, self.num_radial_filtrations_)
        cvtda.logging.logger().print(f"Redial center for images of size {width}x{height}. x: {radial_x}, y: {radial_y}")

        self.filtration_extractors_ = [ ]
        for binarizer_threshold in self.binarizer_thresholds_:
            self._add_height_filtrations(binarizer_threshold)
            self._add_radial_filtrations(binarizer_threshold, radial_x, radial_y)
            self._add_dilation_filtrations(binarizer_threshold)
            self._add_erosion_filtrations(binarizer_threshold)
            self._add_signed_distance_filtrations(binarizer_threshold)
            self._add_density_filtrations(binarizer_threshold)

    def _add_height_filtrations(self, binarizer_threshold: float):
        for direction in self.height_filtration_directions_:
            name = f'/{int(binarizer_threshold * 10)}/HeightFiltrartion_{direction[0]}_{direction[1]}'
            extractor = FiltrationExtractor(
                gtda.images.HeightFiltration,
                { 'direction': numpy.array(direction) },
                binarizer_threshold,
                n_jobs = self.n_jobs_,
                reduced = self.reduced_
            )
            self.filtration_extractors_.append((extractor, name))
            
    def _add_radial_filtrations(self, binarizer_threshold: float, radial_x: typing.List[int], radial_y: typing.List[int]):
        for center in list(itertools.product(radial_x, radial_y)):
            name = f'/{int(binarizer_threshold * 10)}/RadialFiltration_{center[0]}_{center[1]}'
            extractor = FiltrationExtractor(
                gtda.images.RadialFiltration,
                { 'center': numpy.array(center) },
                binarizer_threshold,
                n_jobs = self.n_jobs_,
                reduced = self.reduced_
            )
            self.filtration_extractors_.append((extractor, name))

    def _add_dilation_filtrations(self, binarizer_threshold: float):
        name = f'/{int(binarizer_threshold * 10)}/DilationFiltration'
        extractor = FiltrationExtractor(
            gtda.images.DilationFiltration,
            { }, 
            binarizer_threshold,
            n_jobs = self.n_jobs_,
            reduced = self.reduced_
        )
        self.filtration_extractors_.append((extractor, name))

    def _add_erosion_filtrations(self, binarizer_threshold: float):
        name = f'/{int(binarizer_threshold * 10)}/ErosionFiltration'
        extractor = FiltrationExtractor(
            gtda.images.ErosionFiltration,
            { }, 
            binarizer_threshold,
            n_jobs = self.n_jobs_,
            reduced = self.reduced_
        )
        self.filtration_extractors_.append((extractor, name))

    def _add_signed_distance_filtrations(self, binarizer_threshold: float):
        name = f'/{int(binarizer_threshold * 10)}/SignedDistanceFiltration'
        extractor = FiltrationExtractor(
            gtda.images.SignedDistanceFiltration,
            { }, 
            binarizer_threshold,
            n_jobs = self.n_jobs_,
            reduced = self.reduced_
        )
        self.filtration_extractors_.append((extractor, name))
        
    def _add_density_filtrations(self, binarizer_threshold: float):
        for radius in self.density_filtration_radiuses_:
            name = f'/{int(binarizer_threshold * 10)}/DensityFiltration_{radius}'
            extractor = FiltrationExtractor(
                gtda.images.DensityFiltration,
                { 'radius': radius },
                binarizer_threshold,
                n_jobs = self.n_jobs_,
                reduced = self.reduced_
            )
            self.filtration_extractors_.append((extractor, name))


    def dump_name_concat_(self, dump_name: typing.Optional[str], extra_path: str):
        if dump_name is None:
            return None
        return dump_name + "/" + extra_path
