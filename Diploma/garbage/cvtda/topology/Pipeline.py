import abc
import typing

import numpy
import sklearn.base

import cvtda.utils
import cvtda.dumping
import cvtda.logging


class Pipeline(sklearn.base.TransformerMixin):
    def __init__(
        self,
        n_jobs: int = -1,
        reduced: bool = True,
        only_get_from_dump: bool = False,
        **kwargs
    ):
        self.n_jobs_ = n_jobs
        self.reduced_ = reduced
        self.only_get_from_dump_ = only_get_from_dump
        self.return_diagrams_ = return_diagrams

        self.kwargs_ = kwargs
        self.kwargs_['n_jobs'] = n_jobs
        self.kwargs_['reduced'] = reduced
        # self.kwargs_['only_get_from_dump'] = only_get_from_dump
        # self.kwargs_['return_diagrams'] = return_diagrams

        self.fitted_ = False
        self.fit_dimensions_ = None
    

    def final_dump_name_(self, dump_name: typing.Optional[str] = None):
        return self.features_dump_(dump_name)
    
    def features_dump_(self, dump_name: typing.Optional[str]):
        return (None if dump_name is None else f"{dump_name}/features")
    

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
                self.gray_extractor_ = self.__class__(**self.kwargs_)
                self.red_extractor_ = self.__class__(**self.kwargs_)
                self.green_extractor_ = self.__class__(**self.kwargs_)
                self.blue_extractor_ = self.__class__(**self.kwargs_)
            
            gray_images = cvtda.utils.rgb2gray(images, self.n_jobs_)
            result = [
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
            ]
            if not self.return_diagrams_:
                result = numpy.hstack(result)
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

    def maybe_get_dump_(self, do_fit: bool, dump_name: typing.Optional[str] = None) -> typing.Optional[numpy.ndarray]:
        if do_fit or (dump_name is None) or (not cvtda.dumping.dumper().has_dump(dump_name)):
            return None
        return cvtda.dumping.dumper().get_dump(dump_name)
    
    def dump_name_concat_(self, dump_name: typing.Optional[str], extra_path: str):
        if dump_name is None:
            return None
        return dump_name + "/" + extra_path