import typing

import numpy
import itertools
import sklearn.base

from cvtda.logging import DevNullLogger, CLILogger
from cvtda.dumping import DevNullDumper, NumpyDumper
from cvtda.dumping.base import BaseDumper
from cvtda.utils import set_random_seed
from cvtda.topology import GreyscaleToFiltrations, GreyscaleToPointClouds
from cvtda.topology import FiltrationsToDiagrams, PointCloudsToDiagrams
from cvtda.topology import DiagramsToFeatures

class PointCloudsExtractor(sklearn.base.TransformerMixin):
    def __init__(
        self,
        n_jobs: int = -1,
        verbose: bool = True,
        random_state: int = 42,
        dump_directory: typing.Optional[str] = None,

        binarizer_threshold: float = 0.1,

        radial_filtration_centers_x: typing.List[float] = [ 3, 8, 13, 18, 23 ],
        radial_filtration_centers_y: typing.List[float] = [ 3, 8, 13, 18, 23 ]
    ):
        self.fitted_ = False
        self.n_jobs_ = n_jobs
        self.random_state_ = random_state
        set_random_seed(self.random_state_)
        self.dump_directory_ = dump_directory
        self.logger_ = CLILogger() if verbose else DevNullLogger()

        self.greyscale_to_pointclouds_ = GreyscaleToPointClouds(binarizer_threshold = binarizer_threshold)
        self.pointclouds_flatten_ = FlattenBatch()
        self.pointclouds_to_diagrams_ = PointCloudsToDiagrams()
        self.pointclouds_features_ = DiagramsToFeatures(batch_size = 256)

    def fit(self, images: numpy.ndarray, dumper_id: typing.Optional[str] = None):
        self.process_(images, do_fit = True, dumper_id = dumper_id)
        self.fitted_ = True
        return self
    
    def transform(self, images: numpy.ndarray, dumper_id: typing.Optional[str] = None) -> numpy.ndarray:
        assert self.fitted_ is True, 'fit() must be called before transform()'
        return self.process_(images, do_fit = False, dumper_id = dumper_id)
    
    def fit_transform(self, images: numpy.ndarray, dumper_id: typing.Optional[str] = None) -> numpy.ndarray:
        return self.fit(images, dumper_id = dumper_id).transform(images, dumper_id = dumper_id)
        


    def process_iter_(self, transformer: sklearn.base.TransformerMixin, data: numpy.ndarray, do_fit: bool):
        if do_fit:
            transformer.fit(data)
        return transformer.transform(data)
            
    def process_iter_dump_(
        self,
        transformer: sklearn.base.TransformerMixin,
        data: numpy.ndarray,
        do_fit: bool,
        dumper: BaseDumper,
        dumper_name: str
    ):
        if do_fit:
            transformer.fit(data)
        return dumper.execute(lambda: transformer.transform(data), dumper_name)

    def process_(self, images: numpy.ndarray, do_fit: bool, dumper_id: typing.Optional[str] = None) -> numpy.ndarray:
        dumper = (
            NumpyDumper(f"{self.dump_directory_}/{dumper_id}", self.logger_)
            if self.dump_directory_
            else DevNullDumper()
        )
        set_random_seed(self.random_state_)
        out_shape = (len(images), -1)

        self.logger_.print("> Filtrations")
        filtrations = self.process_iter_(self.greyscale_to_filtrations_, images, do_fit)
        filtrations = self.process_iter_(self.filtrations_flatten_, filtrations, do_fit)
        filtration_diagrams = self.process_iter_dump_(self.filtrations_to_diagrams_, filtrations, do_fit, dumper, "filtration_diagrams")
        del filtrations
        self.logger_.print(f"> Filtration diagrams shape: {filtration_diagrams.shape}")

        self.logger_.print("> Filtration features")
        filtration_features = self.process_iter_dump_(self.filtration_features_, filtration_diagrams, do_fit, dumper, "filtration_features")
        self.logger_.print(f"> Filtration features shape: {filtration_features.shape}")
        del filtration_diagrams
        filtration_features = self.filtrations_flatten_.inverse_transform(filtration_features).reshape(out_shape)

        if not self.with_pointclouds_:
            self.logger_.print(f"> Features shape: {filtration_features.shape}")
            return filtration_features
        
        self.logger_.print("> Point clouds")
        pointclouds = self.process_iter_(self.greyscale_to_pointclouds_, images, do_fit)
        pointclouds = self.process_iter_(self.pointclouds_flatten_, pointclouds, do_fit)
        pointclouds_diagrams = self.process_iter_dump_(self.pointclouds_to_diagrams_, pointclouds, do_fit, dumper, "pointclouds_diagrams")
        del pointclouds
        self.logger_.print(f"> Point cloud diagrams shape: {filtration_diagrams.shape}")

        self.logger_.print("> Point cloud features")
        pointclouds_features = self.process_iter_dump_(self.pointclouds_features_, pointclouds_diagrams, do_fit, dumper, "pointclouds_features")
        self.logger_.print(f"> Point cloud features shape: {filtration_features.shape}")
        del pointclouds_diagrams
        pointclouds_features = self.pointclouds_flatten_.inverse_transform(pointclouds_features).reshape(out_shape)

        self.logger_.print(f"> Features shape: {filtration_features.shape} + {pointclouds_features.shape}")
        return numpy.hstack([ filtration_features, pointclouds_features ])
