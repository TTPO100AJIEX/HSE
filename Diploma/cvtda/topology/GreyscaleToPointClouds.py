import typing

import numpy
import gtda.images
import sklearn.base

from cvtda.utils import set_random_seed
from cvtda.logging import DevNullLogger, CLILogger

class GreyscaleToPointClouds(sklearn.base.TransformerMixin):
    def __init__(
        self,
        n_jobs: int = -1,
        verbose: bool = True,
        random_state: int = 42,

        binarizer_threshold: float = 0.1,
        inverted_binarizer_threshold: float = 0.1,
    ):
        self.fitted_ = False
        self.n_jobs_ = n_jobs
        self.random_state_ = random_state
        set_random_seed(self.random_state_)
        self.logger_ = CLILogger() if verbose else DevNullLogger()

        self.binarizer_ = gtda.images.Binarizer(threshold = binarizer_threshold, n_jobs = n_jobs)

        self.inverter_ = gtda.images.Inverter(n_jobs = n_jobs)
        self.inverted_binarizer_ = gtda.images.Binarizer(threshold = inverted_binarizer_threshold, n_jobs = n_jobs)

        self.image_to_point_cloud_ = gtda.images.ImageToPointCloud(n_jobs = n_jobs)
        self.inverted_image_to_point_cloud_ = gtda.images.ImageToPointCloud(n_jobs = n_jobs)
        
    def fit(self, images: numpy.ndarray):
        set_random_seed(self.random_state_)
        
        self.logger_.print('Fitting the binarizer')
        images_bin = self.binarizer_.fit_transform(images)

        self.logger_.print('Fitting the inverter')
        images_inv = self.inverter_.fit_transform(images)
        images_inv_bin = self.inverted_binarizer_.fit_transform(images_inv)

        self.logger_.print('Fitting the converter')
        self.converter_theshold_ = numpy.max(images) * self.binarizer_.threshold
        self.inverted_converter_theshold_ = numpy.max(images_inv) * self.inverted_binarizer_.threshold

        self.logger_.print('Fitting image_to_point_cloud')
        self.image_to_point_cloud_.fit(images_bin)
        self.inverted_image_to_point_cloud_.fit(images_inv_bin)
        
        self.logger_.print('Fitting complete')
        self.fitted_ = True
        return self
    
    def transform(self, images: numpy.ndarray) -> typing.List[typing.List[numpy.ndarray]]:
        assert self.fitted_ is True
        set_random_seed(self.random_state_)

        self.logger_.print('Applying the binarizer')
        images_bin = self.binarizer_.transform(images)

        self.logger_.print('Applying the inverter')
        images_inv = self.inverter_.transform(images)
        images_inv_bin = self.inverted_binarizer_.transform(images_inv)

        point_cloud = self.make_point_clouds_(
            images, self.converter_theshold_,
            desc = 'Converting images to point clouds'
        )
        inv_point_cloud = self.make_point_clouds_(
            images_inv, self.converter_theshold_,
            desc = 'Converting inverted images to point clouds'
        )

        self.logger_.print('Converting binary to point clouds')
        bin_point_cloud = self.image_to_point_cloud_.transform(images_bin)
        inverted_bin_point_cloud = self.inverted_image_to_point_cloud_.transform(images_inv_bin)

        point_clouds = (point_cloud, inv_point_cloud, bin_point_cloud, inverted_bin_point_cloud)
        return list(self.logger_.zip(*point_clouds, desc = 'Combining the point clouds'))
    

    def make_point_clouds_(
        self,
        images: numpy.ndarray,
        threshold: float,
        desc: str = None
    ) -> typing.List[numpy.ndarray]:
        images = numpy.swapaxes(numpy.flip(images, axis = 1), 1, 2)
        return [
            self.make_point_cloud_(image, threshold)
            for image in self.logger_.loop(images, desc = desc)
        ]
    
    def make_point_cloud_(self, image: numpy.ndarray, threshold: float) -> numpy.ndarray:
        x = numpy.indices(image.shape)[0].flatten()
        y = numpy.indices(image.shape)[1].flatten()
        image = image.flatten()

        mask = (image >= threshold)
        return numpy.vstack([ x[mask], y[mask], image[mask] ]).transpose()
