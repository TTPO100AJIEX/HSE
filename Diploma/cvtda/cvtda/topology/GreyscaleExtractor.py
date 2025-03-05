import typing

import numpy
import gtda.images
import gtda.homology

import cvtda.utils
import cvtda.logging

from .Pipeline import Pipeline
from .DiagramVectorizer import DiagramVectorizer


class GreyscaleExtractor(Pipeline):
    def __init__(self, n_jobs: int = -1, reduced: bool = True):
        super().__init__(n_jobs = n_jobs, reduced = reduced)

        self.persistence_ = gtda.homology.CubicalPersistence(homology_dimensions = [0, 1], n_jobs = self.n_jobs_)
        self.vectorizer_ = DiagramVectorizer(n_jobs = self.n_jobs_, reduced = self.reduced_)
        
        self.inverter_ = gtda.images.Inverter(n_jobs = n_jobs)
        self.inverted_persistence_ = gtda.homology.CubicalPersistence(homology_dimensions = [0, 1], n_jobs = self.n_jobs_)
        self.inverted_vectorizer_ = DiagramVectorizer(n_jobs = self.n_jobs_, reduced = self.reduced_)


    def process_rgb_(self, rgb_images: numpy.ndarray, do_fit: bool, dump_name: typing.Optional[str] = None) -> numpy.ndarray:
        return numpy.zeros((len(rgb_images), 0))
    
    def process_gray_(self, gray_images: numpy.ndarray, do_fit: bool, dump_name: typing.Optional[str] = None) -> numpy.ndarray:
        cvtda.logging.logger().print(f"GreyscaleExtractor: processing {dump_name}, do_fit = {do_fit}")
        
        dump_name_diagrams = self.dump_name_concat_(dump_name, "diagrams")
        dump_name_features = self.dump_name_concat_(dump_name, "features")
        dump_name_inverted_diagrams = self.dump_name_concat_(dump_name, "inverted_diagrams")
        dump_name_inverted_features = self.dump_name_concat_(dump_name, "inverted_features")
        
        inverted_images = self.process_iter_(self.inverter_, gray_images, do_fit)
        
        diagrams = self.process_iter_dump_(self.persistence_, gray_images, do_fit, dump_name_diagrams)
        inverted_diagrams = self.process_iter_dump_(self.persistence_, inverted_images, do_fit, dump_name_inverted_diagrams)

        return numpy.hstack([
            self.process_iter_dump_(self.vectorizer_, diagrams, do_fit, dump_name_features),
            self.process_iter_dump_(self.inverted_vectorizer_, inverted_diagrams, do_fit, dump_name_inverted_features)
        ])
