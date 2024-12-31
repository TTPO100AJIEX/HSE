import typing

import tqdm

from .base import BaseLogger

T = typing.TypeVar("T")

class CLILogger(BaseLogger):
    def __init__(self):
        pass

    def print(self, data: T) -> None:
        print(data)

    def progress_bar(self, data: typing.Iterable[T], total: int = None, desc: typing.Optional[str] = None) -> typing.Iterable[T]:
        return tqdm.tqdm(data, total = total, desc = desc)
