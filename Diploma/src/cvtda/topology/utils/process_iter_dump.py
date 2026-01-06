import typing

import numpy
import sklearn.base

import cvtda.dumping
from .process_iter import process_iter

def process_iter_dump(
    transformer: sklearn.base.TransformerMixin,
    data: numpy.ndarray,
    do_fit: bool,
    dump_name: typing.Optional[str] = None,
    *args,
    **kwargs
):
    function = lambda: process_iter(transformer, data, do_fit, *args, **kwargs)
    return function() if dump_name is None else cvtda.dumping.dumper().execute(function, dump_name)
