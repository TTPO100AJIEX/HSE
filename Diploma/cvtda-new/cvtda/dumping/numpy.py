import os
import typing

import numpy

from .base import BaseDumper
from cvtda.logging import DevNullLogger
from cvtda.logging.base import BaseLogger

class NumpyDumper(BaseDumper[numpy.ndarray]):
    def __init__(self, directory: str, logger: BaseLogger = DevNullLogger()):
        self.directory_ = directory
        self.logger_ = logger
    
    def execute(self, function: typing.Callable[[typing.Any], numpy.ndarray], name: str, *function_args) -> numpy.ndarray:
        file = os.path.join(self.directory_, f"{name}.npy")
        if os.path.exists(file):
            self.logger_.print(f"Got the result from {file}")
            return numpy.load(file)
        
        result = function(*function_args)
        self.logger_.print(f"Saving the result to {file}")
        os.makedirs(self.directory_, exist_ok = True)
        numpy.save(file, result)
        return result
