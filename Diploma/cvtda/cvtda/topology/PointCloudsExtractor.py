import typing

import numpy
import gtda.homology

import cvtda.utils
import cvtda.logging

from .Pipeline import Pipeline
from .DiagramVectorizer import DiagramVectorizer


class PointCloudsExtractor(Pipeline):
    def __init__(self, n_jobs: int = -1, reduced: bool = True):
        super().__init__(n_jobs = n_jobs, reduced = reduced)

        self.persistence_ = gtda.homology.VietorisRipsPersistence(homology_dimensions = [0, 1, 2], n_jobs = self.n_jobs_)
        self.vectorizer_ = DiagramVectorizer(n_jobs = self.n_jobs_, reduced = self.reduced_)


    def process_rgb_(self, *args, **kwargs):
        return self.process_any_(*args, **kwargs)
    
    def process_gray_(self, *args, **kwargs):
        return self.process_any_(*args, **kwargs)
    
    def process_any_(self, images: numpy.ndarray, do_fit: bool, dump_name: typing.Optional[str] = None) -> numpy.ndarray:
        cvtda.logging.logger().print(f"PointCloudsExtractor: processing {dump_name}, do_fit = {do_fit}")
        point_clouds = cvtda.utils.image2pointcloud(images, self.n_jobs_)
        diagrams = self.process_iter_dump_(
            self.persistence_,
            point_clouds,
            do_fit,
            self.dump_name_concat_(dump_name, "diagrams")
        )
        return self.process_iter_dump_(
            self.vectorizer_,
            diagrams,
            do_fit,
            self.dump_name_concat_(dump_name, "features")
        )
