import typing
import itertools

import numpy
import sklearn.base

T = typing.TypeVar("T")

class FlattenBatch(sklearn.base.TransformerMixin):
    fitted_ = False
    
    def fit(self, data: typing.Iterable[typing.Iterable[T]]):
        assert numpy.all([ len(item) == len(data[0]) for item in data ]), \
                'The batch dimensions must be homogeneous'
        self.dimension_ = len(data[0])
        self.fitted_ = True
        return self
    
    def transform(self, data: typing.Iterable[typing.Iterable[T]]) -> typing.Iterable[T]:
        assert self.fitted_ is True, 'fit() must be called before transform()'

        if type(data) == numpy.ndarray:
            return data.reshape((-1, *data.shape[2:]))
        else:
            assert numpy.all([ len(item) == self.dimension_ for item in data ]), \
                    'The second dimension must match the value seen in fit()'
            return list(itertools.chain(*data))
    
    def inverse_transform(self, data: typing.Iterable[T]) -> typing.Iterable[typing.Iterable[T]]:
        assert self.fitted_ is True, 'fit() must be called before inverse_transform()'
        assert len(data) % self.dimension_ == 0, 'It is impossible to split the data into even batches'

        if type(data) == numpy.ndarray:
            return data.reshape((-1, self.dimension_, *data.shape[1:]))
        else:
            return [
                data[i:i + self.dimension_]
                for i in range(0, len(data), self.dimension_)
            ]
