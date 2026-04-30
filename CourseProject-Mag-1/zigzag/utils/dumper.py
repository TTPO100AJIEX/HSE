import os
import typing
import pathlib
import inspect

import numpy
import torch
import pandas

import cvtda.logging

from cvtda.dumping import BaseDumper

T = typing.TypeVar("T")


class UniversalDumper(BaseDumper[torch.Tensor]):
    EXTENSIONS = {
        torch.Tensor: "pt",
        numpy.ndarray: "npy",
        pandas.DataFrame: "csv",
        typing.List[torch.Tensor]: "",
        typing.List[numpy.ndarray]: "",
        list: "",
    }

    def __init__(self, directory: str):
        self.directory_ = directory

    def get_file_name_(self, name: str, ext: typing.Optional[str]):
        name = f"{name}.{ext}" if ext != "" else name
        return os.path.join(self.directory_, name)

    def get_existing_file_name_(self, name: str, ext: typing.Optional[str] = None) -> typing.Optional[str]:
        if ext is not None:
            file = self.get_file_name_(name, ext)
            return file if os.path.exists(file) else None
        exist = set(
            [ext for _, ext in UniversalDumper.EXTENSIONS.items() if os.path.exists(self.get_file_name_(name, ext))]
        )
        assert len(exist) <= 1, f"Ambiguous dump. Multiple files exist: {exist}"
        return next(iter(exist)) if len(exist) == 1 else None

    def execute(self, function: typing.Callable[[typing.Any], T], name: str, *function_args, **function_kwargs) -> T:
        return_type = inspect.signature(function).return_annotation
        ext = UniversalDumper.EXTENSIONS[return_type]
        if self.has_dump(name, ext):
            return self.get_dump(name, ext)
        result = function(*function_args, **function_kwargs)
        self.save_dump(result, name)
        return result

    def save_dump(self, data: T, name: str):
        file = self.get_file_name_(name, UniversalDumper.EXTENSIONS[type(data)])
        cvtda.logging.logger().print(f"Saving the result to {file}")
        os.makedirs(os.path.dirname(file), exist_ok=True)
        if isinstance(data, list):
            for i, item in enumerate(data):
                self.save_dump(item, f"{name}/{i}")
            return
        match type(data):
            case torch.Tensor:
                torch.save(data, file)
            case numpy.ndarray:
                numpy.save(file, data)
            case pandas.DataFrame:
                data.to_csv(file, index=False)
            case __:
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
        else:
            assert False, f"Unsuppported dump filename: {file}"
