import pytest

from ru.hse.ServerResponse import ServerResponse
from ru.hse.IPasswordEncoder import IPasswordEncoder
from ru.hse.IPasswordEncoder import NullPointerException
from ru.hse.AccountManagerResponse import AccountManagerResponse

LOGIN = "tester"
PASSWORD = "123321"
SESSION_ID = 1234567
PASSWORD_HASH = "abcdefghijklmnopqrstuvwxyz"

@pytest.fixture
def account_manager_logged(account_manager, mock_password_encoder, mock_server):
    mock_password_encoder("makeSecure", ( PASSWORD, ), PASSWORD_HASH)
    mock_server("login", ( LOGIN, PASSWORD_HASH ), ServerResponse.SUCCESS, SESSION_ID)
    account_manager.callLogin(LOGIN, PASSWORD)
    return account_manager


def test_succeed(account_manager_logged, mock_server):
    logout_stats = mock_server("logout", ( SESSION_ID, ), ServerResponse.SUCCESS, None)

    response = account_manager_logged.callLogout(LOGIN, SESSION_ID)
    assert response.code == AccountManagerResponse.SUCCEED
    assert response.response is None

    assert logout_stats['ok'] == True
    assert logout_stats['calls'] == 1

    assert account_manager_logged.activeAccounts == { }

    
@pytest.mark.parametrize([ 'server_code', 'expected_code', 'expected_response' ], [
    # Valid responses
    [ ServerResponse.SUCCESS,         AccountManagerResponse.SUCCEED,         None ],
    [ ServerResponse.NOT_LOGGED,      AccountManagerResponse.NOT_LOGGED,      None ],
    [ ServerResponse.UNDEFINED_ERROR, AccountManagerResponse.UNDEFINED_ERROR, None ],
    # Invalid responses
    [ ServerResponse.ALREADY_LOGGED,             AccountManagerResponse.INCORRECT_RESPONSE, ServerResponse(ServerResponse.ALREADY_LOGGED, None) ],
    [ ServerResponse.NO_USER_INCORRECT_PASSWORD, AccountManagerResponse.INCORRECT_RESPONSE, ServerResponse(ServerResponse.NO_USER_INCORRECT_PASSWORD, None) ],
    [ ServerResponse.NO_MONEY,                   AccountManagerResponse.INCORRECT_RESPONSE, ServerResponse(ServerResponse.NO_MONEY, None) ]
])
def test_different_server_responses(account_manager_logged, mock_server, server_code, expected_code, expected_response):
    logout_stats = mock_server("logout", ( SESSION_ID, ), server_code, None)

    response = account_manager_logged.callLogout(LOGIN, SESSION_ID)
    assert response.code == expected_code
    if isinstance(expected_response, ServerResponse):
        assert isinstance(response.response, ServerResponse)
        assert response.response.code == expected_response.code
        assert response.response.response == expected_response.response
    else:
        assert response.response == expected_response

    assert logout_stats['ok'] == True
    assert logout_stats['calls'] == 1


@pytest.mark.parametrize([ "login", "session_id", "expected_code" ], [
    [ LOGIN, SESSION_ID + 12345, AccountManagerResponse.INCORRECT_SESSION ],
    [ LOGIN + "-wrong", SESSION_ID, AccountManagerResponse.NOT_LOGGED ],
    [ LOGIN + "-wrong", SESSION_ID + 12345, AccountManagerResponse.NOT_LOGGED ]
])
def test_incorrect_session(account_manager_logged, login, session_id, expected_code):
    response = account_manager_logged.callLogout(login, session_id)
    assert response.code == expected_code
    assert response.response is None


def test_no_logged(account_manager):
    response = account_manager.callLogout(LOGIN, SESSION_ID)
    assert response.code == AccountManagerResponse.NOT_LOGGED
    assert response.response is None


def test_double_logout(account_manager_logged, mock_server):
    logout_stats = mock_server("logout", ( SESSION_ID, ), ServerResponse.SUCCESS, None)

    response = account_manager_logged.callLogout(LOGIN, SESSION_ID)
    assert response.code == AccountManagerResponse.SUCCEED
    assert response.response is None

    assert logout_stats['ok'] == True
    assert logout_stats['calls'] == 1
    
    response = account_manager_logged.callLogout(LOGIN, SESSION_ID)
    assert response.code == AccountManagerResponse.NOT_LOGGED
    assert response.response is None

    assert logout_stats['ok'] == True
    assert logout_stats['calls'] == 1