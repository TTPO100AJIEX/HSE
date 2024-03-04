import pytest

from ru.hse.IServer import IServer
from ru.hse.AccountManager import AccountManager
from ru.hse.IPasswordEncoder import IPasswordEncoder

@pytest.fixture
def account_manager(monkeypatch):
    monkeypatch.setattr(IPasswordEncoder, "__abstractmethods__", set())
    monkeypatch.setattr(IServer, "__abstractmethods__", set())
    return AccountManager(IServer(), IPasswordEncoder())