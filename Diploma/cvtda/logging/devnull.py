import typing

from .base import BaseLogger

T = typing.TypeVar("T")

class DevNullLogger(BaseLogger):
    def print(self, data: T) -> None:
        pass

    def progress_bar(self, data: typing.Iterable[T], total: int = None, desc: typing.Optional[str] = None) -> typing.Iterable[T]:
        return data
