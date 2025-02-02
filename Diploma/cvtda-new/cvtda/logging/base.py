import abc
import typing

T = typing.TypeVar("T")

class BaseLogger(abc.ABC):
    @abc.abstractmethod
    def print(self, data: T) -> None:
        pass

    @abc.abstractmethod
    def loop(
        self,
        data: typing.Iterable[T],
        total: typing.Optional[int] = None,
        desc: typing.Optional[str] = None
    ) -> typing.Iterable[T]:
        pass

    @abc.abstractmethod
    def zip(
        self,
        *iterables, 
        desc: typing.Optional[str] = None
    ):
        pass

    @abc.abstractmethod
    def set_pbar_postfix(self, pbar: typing.Any, data: dict):
        pass
