import typing

import numpy
import gtda.images
import sklearn.base

from . import utils
from .FiltrationsExtractor import FiltrationsExtractor
from .GreyscaleExtractor import GreyscaleExtractor
from .PointCloudsExtractor import PointCloudsExtractor
from .GeometryExtractor import GeometryExtractor


class FeatureExtractor(sklearn.base.TransformerMixin):
    def __init__(
        self,
        n_jobs: int = -1,
        reduced: bool = True,
        only_get_from_dump: bool = False,
        return_diagrams: bool = False
    ):
        self.fitted_ = False
        self.reduced_ = reduced
        self.return_diagrams_ = return_diagrams
        
        extractor_kwargs = { 'n_jobs': n_jobs, 'reduced': reduced, 'only_get_from_dump': only_get_from_dump }
        topological_extractor_kwargs = { **extractor_kwargs, 'return_diagrams': return_diagrams }

        self.inverter_ = gtda.images.Inverter()

        self.point_clouds_extractor_ = PointCloudsExtractor(**topological_extractor_kwargs)
        self.greyscale_extractor_ = GreyscaleExtractor(**topological_extractor_kwargs)
        self.inverted_greyscale_extractor_ = GreyscaleExtractor(**topological_extractor_kwargs)
        self.filtrations_extractor_ = FiltrationsExtractor(**topological_extractor_kwargs)
        self.geometry_extractor_ = GeometryExtractor(**extractor_kwargs)

    def fit(self, images: numpy.ndarray, dump_name: typing.Optional[str] = None):
        inverted_images = utils.process_iter(self.inverter_, images, do_fit = True)

        if not self.reduced_:
            self.point_clouds_extractor_.fit(images, utils.dump_name_concat(dump_name, "point_clouds"))

        self.greyscale_extractor_.fit(images, utils.dump_name_concat(dump_name, "greyscale"))
        self.inverted_greyscale_extractor_.fit(inverted_images, utils.dump_name_concat(dump_name, "inverted_greyscale"))
        self.filtrations_extractor_.fit(images, utils.dump_name_concat(dump_name, "filtrations"))

        if not self.return_diagrams_:
            self.geometry_extractor_.fit(images, utils.dump_name_concat(dump_name, "geometry"))

        self.fitted_ = True
        return self
    
    def transform(self, images: numpy.ndarray, dump_name: typing.Optional[str] = None) -> numpy.ndarray:
        assert self.fitted_ is True, 'fit() must be called before transform()'
        
        inverted_images = utils.process_iter(self.inverter_, images, do_fit = False)

        results = []
        if not self.reduced_:
            results.append(self.point_clouds_extractor_.transform(images, utils.dump_name_concat(dump_name, "point_clouds")))

        results.append(self.greyscale_extractor_.transform(images, utils.dump_name_concat(dump_name, "greyscale")))
        results.append(self.inverted_greyscale_extractor_.transform(inverted_images, utils.dump_name_concat(dump_name, "inverted_greyscale")))
        results.append(self.filtrations_extractor_.transform(images, utils.dump_name_concat(dump_name, "filtrations")))
        
        if not self.return_diagrams_:
            results.append(self.geometry_extractor_.transform(images, utils.dump_name_concat(dump_name, "geometry")))
        
        return utils.hstack(results, not self.return_diagrams_)
    
    def fit_transform(self, images: numpy.ndarray, dump_name: typing.Optional[str] = None) -> numpy.ndarray:
        return self.fit(images, dump_name = dump_name).transform(images, dump_name = dump_name)

