import pytest

from ru.hse.ServerResponse import ServerResponse
from ru.hse.AccountManagerResponse import AccountManagerResponse

AMOUNT = 12.25
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

    
@pytest.mark.parametrize([ 'server_code', 'server_response', 'expected_code', 'expected_response' ], [
    # Valid responses
    [ ServerResponse.SUCCESS,         AMOUNT, AccountManagerResponse.SUCCEED,         AMOUNT ],
    [ ServerResponse.NOT_LOGGED,      None,   AccountManagerResponse.NOT_LOGGED,      None ],
    [ ServerResponse.UNDEFINED_ERROR, None,   AccountManagerResponse.UNDEFINED_ERROR, None ],
    # Invalid responses
    [ ServerResponse.SUCCESS,                    None, AccountManagerResponse.INCORRECT_RESPONSE, ServerResponse(ServerResponse.SUCCESS, None) ],
    [ ServerResponse.NO_MONEY,                   None, AccountManagerResponse.INCORRECT_RESPONSE, ServerResponse(ServerResponse.NO_MONEY, None) ],
    [ ServerResponse.ALREADY_LOGGED,             None, AccountManagerResponse.INCORRECT_RESPONSE, ServerResponse(ServerResponse.ALREADY_LOGGED, None) ],
    [ ServerResponse.NO_USER_INCORRECT_PASSWORD, None, AccountManagerResponse.INCORRECT_RESPONSE, ServerResponse(ServerResponse.NO_USER_INCORRECT_PASSWORD, None) ],
])
def test_different_server_responses(
    account_manager_logged, mock_server,
    server_code, server_response, expected_code, expected_response
):
    getbalance_stats = mock_server("getBalance", ( SESSION_ID, ), server_code, server_response)

    response = account_manager_logged.getBalance(LOGIN, SESSION_ID)
    assert response.code == expected_code
    if isinstance(expected_response, ServerResponse):
        assert isinstance(response.response, ServerResponse)
        assert response.response.code == expected_response.code
        assert response.response.response == expected_response.response
    else:
        assert response.response == expected_response

    assert getbalance_stats['ok'] == True
    assert getbalance_stats['calls'] == 1


@pytest.mark.parametrize([ "login", "session_id", "expected_code" ], [
    [ LOGIN, SESSION_ID + 12345, AccountManagerResponse.INCORRECT_SESSION ],
    [ LOGIN + "-wrong", SESSION_ID, AccountManagerResponse.NOT_LOGGED ],
    [ LOGIN + "-wrong", SESSION_ID + 12345, AccountManagerResponse.NOT_LOGGED ]
])
def test_incorrect_session(account_manager_logged, login, session_id, expected_code):
    response = account_manager_logged.getBalance(login, session_id)
    assert response.code == expected_code
    assert response.response is None


def test_no_logged(account_manager):
    response = account_manager.getBalance(LOGIN, SESSION_ID)
    assert response.code == AccountManagerResponse.NOT_LOGGED
    assert response.response is None