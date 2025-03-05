from .base import BaseDumper
from .numpy import NumpyDumper
from .devnull import DevNullDumper

def dumper() -> BaseDumper:
    if BaseDumper.current_dumper is None:
        BaseDumper.current_dumper = DevNullDumper()
    return BaseDumper.current_dumper
