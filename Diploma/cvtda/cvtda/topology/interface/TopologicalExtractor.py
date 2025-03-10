import abc
import typing

import numpy

from .. import utils
from .Extractor import Extractor
from ..DiagramVectorizer import DiagramVectorizer

class TopologicalExtractor(Extractor):
    def __init__(
        self,
        supports_rgb: bool,
        n_jobs: int = -1,
        reduced: bool = True,
        only_get_from_dump: bool = False,
        return_diagrams: bool = False,
        **kwargs
    ):
        super().__init__(
            n_jobs = n_jobs,
            reduced = reduced,
            only_get_from_dump = only_get_from_dump,
            return_diagrams = return_diagrams,
            supports_rgb = supports_rgb,
            **kwargs
        )

        self.return_diagrams_ = return_diagrams
        self.supports_rgb_ = supports_rgb

        self.vectorizer_ = DiagramVectorizer(n_jobs = self.n_jobs_, reduced = self.reduced_)
        

    def final_dump_name_(self, dump_name: typing.Optional[str] = None):
        return self.diagrams_dump_(dump_name) if self.return_diagrams_ else self.features_dump_(dump_name)
    
    def diagrams_dump_(self, dump_name: typing.Optional[str]):
        return utils.dump_name_concat(dump_name, "diagrams")
    
    def force_numpy_(self):
        return not self.return_diagrams_


    def process_rgb_(self, rgb_images: numpy.ndarray, do_fit: bool, dump_name: typing.Optional[str] = None):
        if not self.supports_rgb_:
            return []

        diagrams = self.get_diagrams_(rgb_images, do_fit, dump_name)
        if self.return_diagrams_:
            return diagrams
        return utils.process_iter_dump(self.vectorizer_, diagrams, do_fit, self.features_dump_(dump_name))

    def process_gray_(self, gray_images: numpy.ndarray, do_fit: bool, dump_name: typing.Optional[str] = None):
        diagrams = self.get_diagrams_(gray_images, do_fit, dump_name)
        if self.return_diagrams_:
            return diagrams
        return utils.process_iter_dump(self.vectorizer_, diagrams, do_fit, self.features_dump_(dump_name))
    
    @abc.abstractmethod
    def get_diagrams_(self, images: numpy.ndarray, do_fit: bool, dump_name: typing.Optional[str] = None):
        pass