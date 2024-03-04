from ru.hse.IServer import IServer
from ru.hse.IPasswordEncoder import IPasswordEncoder

LOGIN = "Tester"
PASSWORD = "123321"

def mock_make_secure(password: str) -> str:
    assert password == PASSWORD

def test_login_ok(account_manager, monkeypatch):
    account_manager.callLogin(LOGIN, PASSWORD)
    assert account_manager is None