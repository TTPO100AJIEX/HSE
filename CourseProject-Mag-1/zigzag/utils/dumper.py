import os
import typing
import pathlib
import inspect

import numpy
import torch
import pandas
import scipy.sparse

import cvtda.logging

from cvtda.dumping import BaseDumper

T = typing.TypeVar("T")

EXTENSIONS = ["pt", "npy", "csv", "npz", ""]


def get_data_type(data) -> type:
    if type(data) == "type":
        return data
    if type(data) == list:
        return typing.List
    return type(data)


def get_extension(data_type):
    if data_type.__name__ == "List" or data_type == list:
        return ""
    if data_type == torch.Tensor:
        return "pt"
    if data_type == numpy.ndarray:
        return "npy"
    if data_type == pandas.DataFrame:
        return "csv"
    if data_type == scipy.sparse.csr_matrix:
        return "npz"
    assert False, f"Unsuppported data type: {data_type}"


class UniversalDumper(BaseDumper[torch.Tensor]):
    def __init__(self, directory: str):
        self.directory_ = directory

    def get_file_name_(self, name: str, ext: typing.Optional[str]):
        name = f"{name}.{ext}" if ext != "" else name
        return os.path.join(self.directory_, name)

    def get_existing_file_name_(self, name: str, ext: typing.Optional[str] = None) -> typing.Optional[str]:
        if ext is not None:
            file = self.get_file_name_(name, ext)
            return file if os.path.exists(file) else None
        exist = set([ext for ext in EXTENSIONS if os.path.exists(self.get_file_name_(name, ext))])
        assert len(exist) <= 1, f"Ambiguous dump. Multiple files exist: {exist}"
        return next(iter(exist)) if len(exist) == 1 else None

    def execute(self, function: typing.Callable[[typing.Any], T], name: str, *function_args, **function_kwargs) -> T:
        return_type = inspect.signature(function).return_annotation
        if self.has_dump(name, get_extension(return_type)):
            return self.get_dump(name, get_extension(return_type))
        result = function(*function_args, **function_kwargs)
        self.save_dump(result, name)
        return result

    def save_dump(self, data: T, name: str):
        file = self.get_file_name_(name, get_extension(get_data_type(data)))
        cvtda.logging.logger().print(f"Saving the result to {file}")
        os.makedirs(os.path.dirname(file), exist_ok=True)

        data_type = get_data_type(data)
        if data_type.__name__ in ("list", "List"):
            for i, item in enumerate(data):
                self.save_dump(item, f"{name}/{i}")
            return
        if data_type == torch.Tensor:
            return torch.save(data, file)
        if data_type == numpy.ndarray:
            return numpy.save(file, data)
        if data_type == pandas.DataFrame:
            return data.to_csv(file, index=False)
        if data_type == scipy.sparse.csr_matrix:
            return scipy.sparse.save_npz(file, data)
        assert False, f"Unsuppported data type: {type(data)}"

    def has_dump(self, name: str, ext: typing.Optional[str] = None) -> bool:
        return self.get_existing_file_name_(name, ext) is not None

    def get_dump_impl_(self, name: str, ext: typing.Optional[str] = None) -> T:
        file = self.get_existing_file_name_(name, ext)
        cvtda.logging.logger().print(f"Got the result from {file}")
        if file.endswith(name):

            def get_subfolder_dump(filename: str):
                path = pathlib.Path(filename)
                return self.get_dump(f"{name}/{path.stem}", path.suffix[1:])

            def file_key(filename):
                return int(pathlib.Path(filename).stem)

            return [get_subfolder_dump(filename) for filename in sorted(os.listdir(file), key=file_key)]
        elif file.endswith(".pt"):
            return torch.load(file)
        elif file.endswith(".npy"):
            return numpy.load(file)
        elif file.endswith("csv"):
            return pandas.read_csv(file)
        elif file.endswith(".npz"):
            return scipy.sparse.load_npz(file)
        else:
            assert False, f"Unsuppported dump filename: {file}"
