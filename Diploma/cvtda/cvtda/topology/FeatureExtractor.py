import abc
import typing

import numpy
import sklearn.base

from .FiltrationsExtractor import FiltrationsExtractor
from .GreyscaleExtractor import GreyscaleExtractor
from .PointCloudsExtractor import PointCloudsExtractor
from .GeometryExtractor import GeometryExtractor


class FeatureExtractor(sklearn.base.TransformerMixin):
    def __init__(self, n_jobs: int = -1, reduced: bool = True):
        self.fitted_ = False
        self.reduced_ = reduced

        self.point_clouds_extractor_ = PointCloudsExtractor(n_jobs = n_jobs, reduced = reduced)
        self.greyscale_extractor_ = GreyscaleExtractor(n_jobs = n_jobs, reduced = reduced)
        self.filtrations_extractor_ = FiltrationsExtractor(n_jobs = n_jobs, reduced = reduced)
        self.geometry_extractor_ = GeometryExtractor(n_jobs = n_jobs, reduced = reduced)

    def fit(self, images: numpy.ndarray, dump_name: typing.Optional[str] = None):
        if not self.reduced_:
            self.point_clouds_extractor_.fit(images, self.dump_name_concat_(dump_name, "point_clouds"))

        self.greyscale_extractor_.fit(images, self.dump_name_concat_(dump_name, "greyscale"))
        self.filtrations_extractor_.fit(images, self.dump_name_concat_(dump_name, "filtrations"))
        self.geometry_extractor_.fit(images, self.dump_name_concat_(dump_name, "geometry"))

        self.fitted_ = True
        return self
    
    def transform(self, images: numpy.ndarray, dump_name: typing.Optional[str] = None) -> numpy.ndarray:
        assert self.fitted_ is True, 'fit() must be called before transform()'

        features = []
        if not self.reduced_:
            features.append(self.point_clouds_extractor_.transform(images, self.dump_name_concat_(dump_name, "point_clouds")))

        features.append(self.greyscale_extractor_.transform(images, self.dump_name_concat_(dump_name, "greyscale")))
        features.append(self.filtrations_extractor_.transform(images, self.dump_name_concat_(dump_name, "filtrations")))
        features.append(self.geometry_extractor_.transform(images, self.dump_name_concat_(dump_name, "geometry")))
        
        return numpy.hstack(features)
    
    def fit_transform(self, images: numpy.ndarray, dump_name: typing.Optional[str] = None) -> numpy.ndarray:
        return self.fit(images, dump_name = dump_name).transform(images, dump_name = dump_name)


    def dump_name_concat_(self, dump_name: typing.Optional[str], extra_path: str):
        if dump_name is None:
            return None
        return dump_name + "/" + extra_path
