import abc
import typing
import importlib

import cvtda.logging

T = typing.TypeVar("T")

class BaseLogger(abc.ABC):
    __current_logger = None

    @staticmethod
    def get() -> cvtda.logging.BaseLogger:
        if BaseLogger.__current_logger is None:
            module = importlib.import_module("cvtda.logging")
            return module.CLILogger()
        return BaseLogger.__current_logger
    
    def __enter__(self):
        self.__previous = BaseLogger.get()

    def __exit__(self, *args):
        BaseLogger.__current_logger = self.__previous

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
