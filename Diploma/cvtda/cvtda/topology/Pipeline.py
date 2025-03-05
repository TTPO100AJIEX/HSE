import abc
import typing

import numpy
import sklearn.base

import cvtda.utils
import cvtda.dumping
import cvtda.logging


class Pipeline(sklearn.base.TransformerMixin):
    def __init__(self, n_jobs: int = -1, reduced: bool = True, **kwargs):
        self.n_jobs_ = n_jobs
        self.reduced_ = reduced
        self.kwargs_ = kwargs

        self.fitted_ = False
        self.fit_dimensions_ = None
    
    def fit(self, images: numpy.ndarray, dump_name: typing.Optional[str] = None):
        self.process_(images, do_fit = True, dump_name = dump_name)
        self.fitted_ = True
        return self
    
    def transform(self, images: numpy.ndarray, dump_name: typing.Optional[str] = None) -> numpy.ndarray:
        assert self.fitted_ is True, 'fit() must be called before transform()'
        return self.process_(images, do_fit = False, dump_name = dump_name)
    
    def fit_transform(self, images: numpy.ndarray, dump_name: typing.Optional[str] = None) -> numpy.ndarray:
        return self.fit(images, dump_name = dump_name).transform(images, dump_name = dump_name)


    def process_(self, images: numpy.ndarray, do_fit: bool, dump_name: typing.Optional[str] = None) -> numpy.ndarray:
        if self.fit_dimensions_ is not None:
            assert self.fit_dimensions_ == images.shape[1:], \
                    f"The pipeline is fit for {self.fit_dimensions_}. Cannot use it with {images.shape}."
        
        if len(images.shape) == 4:
            assert images.shape[3] == 3, f'Images with {len(images.shape)} channels are not supported'
            cvtda.logging.logger().print("RGB images received. Transforming to grayscale.")

            if do_fit:
                self.gray_extractor_ = self.__class__(n_jobs = self.n_jobs_, reduced = self.reduced_, **self.kwargs_)
                self.red_extractor_ = self.__class__(n_jobs = self.n_jobs_, reduced = self.reduced_, **self.kwargs_)
                self.green_extractor_ = self.__class__(n_jobs = self.n_jobs_, reduced = self.reduced_, **self.kwargs_)
                self.blue_extractor_ = self.__class__(n_jobs = self.n_jobs_, reduced = self.reduced_, **self.kwargs_)
            
            gray_images = cvtda.utils.rgb2gray(images, self.n_jobs_)
            result = numpy.hstack([
                self.process_rgb_(
                    images, do_fit, self.dump_name_concat_(dump_name, "rgb")
                ),
                self.process_iter_(
                    self.gray_extractor_, gray_images, do_fit, self.dump_name_concat_(dump_name, "gray")
                ),
                self.process_iter_(
                    self.red_extractor_, images[:, :, :, 0], do_fit, self.dump_name_concat_(dump_name, "red")
                ),
                self.process_iter_(
                    self.green_extractor_, images[:, :, :, 1], do_fit, self.dump_name_concat_(dump_name, "green")
                ),
                self.process_iter_(
                    self.blue_extractor_, images[:, :, :, 2], do_fit, self.dump_name_concat_(dump_name, "blue")
                ),
            ])
        else:
            assert len(images.shape) == 3, f'{len(images.shape) - 1}d images are not supported'
            result = self.process_gray_(images, do_fit, dump_name)
    
        self.fit_dimensions_ = images.shape[1:]
        return result
    
    @abc.abstractmethod
    def process_rgb_(self, rgb_images: numpy.ndarray, do_fit: bool, dump_name: typing.Optional[str] = None) -> numpy.ndarray:
        pass
    
    @abc.abstractmethod
    def process_gray_(self, gray_images: numpy.ndarray, do_fit: bool, dump_name: typing.Optional[str] = None) -> numpy.ndarray:
        pass


    def process_iter_(self, transformer: sklearn.base.TransformerMixin, data: numpy.ndarray, do_fit: bool, *args, **kwargs):
        if do_fit:
            transformer.fit(data, *args, **kwargs)
        return transformer.transform(data, *args, **kwargs)
            
    def process_iter_dump_(
        self,
        transformer: sklearn.base.TransformerMixin,
        data: numpy.ndarray,
        do_fit: bool,
        dump_name: typing.Optional[str] = None,
        *args,
        **kwargs
    ):
        if dump_name is None:
            return self.process_iter_(transformer, data, do_fit, *args, **kwargs)
        
        if do_fit:
            transformer.fit(data, *args, **kwargs)
        return cvtda.dumping.dumper().execute(lambda: transformer.transform(data, *args, **kwargs), dump_name)
    
    def dump_name_concat_(self, dump_name: typing.Optional[str], extra_path: str):
        if dump_name is None:
            return None
        return dump_name + "/" + extra_path
