import pytest

from ru.hse.ServerResponse import ServerResponse
from ru.hse.IPasswordEncoder import IPasswordEncoder
from ru.hse.IPasswordEncoder import NullPointerException
from ru.hse.AccountManagerResponse import AccountManagerResponse

LOGIN = "tester"
PASSWORD = "123321"
SESSION_ID = 1234567
PASSWORD_HASH = "abcdefghijklmnopqrstuvwxyz"


def test_succeed(account_manager, mock_password_encoder, mock_server):
    makesecure_stats = mock_password_encoder("makeSecure", ( PASSWORD, ), PASSWORD_HASH)
    login_stats = mock_server("login", ( LOGIN, PASSWORD_HASH ), ServerResponse.SUCCESS, SESSION_ID)

    response = account_manager.callLogin(LOGIN, PASSWORD)
    assert response.code == AccountManagerResponse.SUCCEED
    assert response.response == SESSION_ID

    assert makesecure_stats['ok'] == True
    assert makesecure_stats['calls'] == 1

    assert login_stats['ok'] == True
    assert login_stats['calls'] == 1

    assert account_manager.activeAccounts == { LOGIN: SESSION_ID }


@pytest.mark.parametrize([ 'server_code', 'server_response', 'expected_code', 'expected_response' ], [
    # Valid responses
    [ ServerResponse.SUCCESS,                    SESSION_ID, AccountManagerResponse.SUCCEED,                    SESSION_ID ],
    [ ServerResponse.ALREADY_LOGGED,             None,       AccountManagerResponse.ALREADY_LOGGED,             None       ],
    [ ServerResponse.NO_USER_INCORRECT_PASSWORD, None,       AccountManagerResponse.NO_USER_INCORRECT_PASSWORD, None       ],
    [ ServerResponse.UNDEFINED_ERROR,            None,       AccountManagerResponse.UNDEFINED_ERROR,            None       ],
    # Invalid responses
    [ ServerResponse.SUCCESS,    "invalid-string-session-id", AccountManagerResponse.INCORRECT_RESPONSE, ServerResponse(ServerResponse.SUCCESS, "invalid-string-session-id") ],
    [ ServerResponse.NOT_LOGGED, None,                        AccountManagerResponse.INCORRECT_RESPONSE, ServerResponse(ServerResponse.NOT_LOGGED, None) ],
    [ ServerResponse.NO_MONEY,   None,                        AccountManagerResponse.INCORRECT_RESPONSE, ServerResponse(ServerResponse.NO_MONEY, None) ]
])
def test_different_server_responses(
    account_manager, mock_password_encoder, mock_server,
    server_code, server_response, expected_code, expected_response
):
    makesecure_stats = mock_password_encoder("makeSecure", ( PASSWORD, ), PASSWORD_HASH)
    login_stats = mock_server("login", ( LOGIN, PASSWORD_HASH ), server_code, server_response)

    response = account_manager.callLogin(LOGIN, PASSWORD)
    assert response.code == expected_code
    if isinstance(expected_response, ServerResponse):
        assert isinstance(response.response, ServerResponse)
        assert response.response.code == expected_response.code
        assert response.response.response == expected_response.response
    else:
        assert response.response == expected_response

    assert makesecure_stats['ok'] == True
    assert makesecure_stats['calls'] == 1

    assert login_stats['ok'] == True
    assert login_stats['calls'] == 1


def test_encoding_error(account_manager, monkeypatch):
    def mock_make_secure(self, password: str):
        raise NullPointerException()
    monkeypatch.setattr(IPasswordEncoder, "makeSecure", mock_make_secure)

    response = account_manager.callLogin(LOGIN, PASSWORD)
    assert response.code == AccountManagerResponse.ENCODING_ERROR
    assert response.response is None


def test_already_logged(account_manager, mock_password_encoder, mock_server):
    makesecure_stats = mock_password_encoder("makeSecure", ( PASSWORD, ), PASSWORD_HASH)
    login_stats = mock_server("login", ( LOGIN, PASSWORD_HASH ), ServerResponse.SUCCESS, SESSION_ID)

    response = account_manager.callLogin(LOGIN, PASSWORD)
    assert response.code == AccountManagerResponse.SUCCEED
    assert response.response == SESSION_ID
    
    response2 = account_manager.callLogin(LOGIN, PASSWORD)
    assert response2.code == AccountManagerResponse.ALREADY_LOGGED
    assert response2.response is None

    assert makesecure_stats['ok'] == True
    assert makesecure_stats['calls'] == 1

    assert login_stats['ok'] == True
    assert login_stats['calls'] == 1

    assert account_manager.activeAccounts == { LOGIN: SESSION_ID }
