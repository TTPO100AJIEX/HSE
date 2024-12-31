import abc
import typing

T = typing.TypeVar("T")

class BaseLogger(abc.ABC):
    @abc.abstractmethod
    def print(self, data: T) -> None:
        pass

    @abc.abstractmethod
    def progress_bar(self, data: typing.Iterable[T], total: int = None, desc: typing.Optional[str] = None) -> typing.Iterable[T]:
        pass
