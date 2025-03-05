import os
import typing

import numpy

import cvtda.logging

from .base import BaseDumper

class NumpyDumper(BaseDumper[numpy.ndarray]):
    def __init__(self, directory: str):
        self.directory_ = directory
    
    def execute(self, function: typing.Callable[[typing.Any], numpy.ndarray], name: str, *function_args) -> numpy.ndarray:
        file = os.path.join(self.directory_, f"{name}.npy")
        if os.path.exists(file):
            cvtda.logging.logger().print(f"Got the result from {file}")
            return numpy.load(file)
        
        result = function(*function_args)
        cvtda.logging.logger().print(f"Saving the result to {file}")
        os.makedirs(os.path.dirname(file), exist_ok = True)
        numpy.save(file, result)
        return result
