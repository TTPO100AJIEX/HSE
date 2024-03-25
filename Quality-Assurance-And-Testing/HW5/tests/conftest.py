import functools

import pytest

from ru.hse.IServer import IServer
from ru.hse.ServerResponse import ServerResponse
from ru.hse.AccountManager import AccountManager
from ru.hse.IPasswordEncoder import IPasswordEncoder


@pytest.fixture
def account_manager(monkeypatch):
    monkeypatch.setattr(IPasswordEncoder, "__abstractmethods__", set())
    monkeypatch.setattr(IServer, "__abstractmethods__", set())
    return AccountManager(IServer(), IPasswordEncoder())


@pytest.fixture
def mock_obj_method(monkeypatch):
    def mock(obj, method_name, expected_args, return_value):
        stats = { 'ok': True, 'calls': 0 }
        def mock_method(self, *args):
            stats['ok'] = False
            assert args == expected_args
            stats['ok'] = True
            stats['calls'] += 1
            return return_value
        monkeypatch.setattr(obj, method_name, mock_method)
        return stats
    return mock

@pytest.fixture
def mock_password_encoder(mock_obj_method):
    return functools.partial(mock_obj_method, IPasswordEncoder)

@pytest.fixture
def mock_server(mock_obj_method):
    def mock(method_name, expected_args, code, response):
        return_value = ServerResponse(code, response)
        return mock_obj_method(IServer, method_name, expected_args, return_value)
    return mock
