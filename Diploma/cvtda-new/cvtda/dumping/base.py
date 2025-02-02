import abc
import typing

T = typing.TypeVar("T")

class BaseDumper(abc.ABC, typing.Generic[T]):
    @abc.abstractmethod
    def execute(self, function: typing.Callable[[typing.Any], T], name: str, *function_args) -> T:
        pass
