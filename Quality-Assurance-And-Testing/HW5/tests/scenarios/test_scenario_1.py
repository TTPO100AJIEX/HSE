from ru.hse.IServer import IServer
from ru.hse.ServerResponse import ServerResponse
from ru.hse.IPasswordEncoder import IPasswordEncoder
from ru.hse.AccountManagerResponse import AccountManagerResponse

def test_scenario_1(account_manager, monkeypatch, mock_server):
    CORRECT_PASSWORD = "correct-password"
    WRONG_PASSWORD = "wrong-password"

    CORRECT_PASSWORD_HASH = "correct_password_hash"
    WRONG_PASSWORD_HASH = "wrong_password_hash"

    CORRECT_LOGIN = "correct-login"
    WRONG_LOGIN = "wrong-login"

    CORRECT_SESSION_ID = 12345

    INITIAL_BALANCE = 1.8
    DEPOSIT_AMOUNT = 100

    def makeSecure(self, password: str):
        if password == CORRECT_PASSWORD: return CORRECT_PASSWORD_HASH
        elif password == WRONG_PASSWORD: return WRONG_PASSWORD_HASH
        else: raise "Password not hashable"
    monkeypatch.setattr(IPasswordEncoder, "makeSecure", makeSecure)

    def login(self, userName: str, mdPass: str):
        if (userName, mdPass) == (WRONG_LOGIN, CORRECT_PASSWORD_HASH):
                return ServerResponse(ServerResponse.NO_USER_INCORRECT_PASSWORD, None)
        elif (userName, mdPass) == (CORRECT_LOGIN, WRONG_PASSWORD_HASH):
                return ServerResponse(ServerResponse.NO_USER_INCORRECT_PASSWORD, None)
        elif (userName, mdPass) == (CORRECT_LOGIN, CORRECT_PASSWORD_HASH):
                return ServerResponse(ServerResponse.SUCCESS, CORRECT_SESSION_ID)
        else: raise "Pair (userName, mdPass) not handled"
    monkeypatch.setattr(IServer, "login", login)

    mock_server("getBalance", ( CORRECT_SESSION_ID, ), ServerResponse.SUCCESS, INITIAL_BALANCE)
    mock_server("deposit", ( CORRECT_SESSION_ID, DEPOSIT_AMOUNT ), ServerResponse.SUCCESS, DEPOSIT_AMOUNT + INITIAL_BALANCE)

    # Пользователь (user) проводит попытку авторизации в системе управления аккаунтами с указанием некорректного логина
    response1 = account_manager.callLogin(WRONG_LOGIN, CORRECT_PASSWORD)
    assert response1.code == AccountManagerResponse.NO_USER_INCORRECT_PASSWORD
    assert response1.response is None

    # После этого он проводит вторую попытку авторизации с указанием корректного логина и неправильного пароля.
    response2 = account_manager.callLogin(CORRECT_LOGIN, WRONG_PASSWORD)
    assert response2.code == AccountManagerResponse.NO_USER_INCORRECT_PASSWORD
    assert response2.response is None

    # С третьей попытки пользователь авторизуется
    response3 = account_manager.callLogin(CORRECT_LOGIN, CORRECT_PASSWORD)
    assert response3.code == AccountManagerResponse.SUCCEED
    assert response3.response == CORRECT_SESSION_ID

    # и делает запрос баланса
    response4 = account_manager.getBalance(CORRECT_LOGIN, CORRECT_SESSION_ID)
    assert response4.code == AccountManagerResponse.SUCCEED
    assert response4.response == INITIAL_BALANCE

    # После получения значения проводится попытка внести на счет 100 единиц
    response5 = account_manager.deposit(CORRECT_LOGIN, CORRECT_SESSION_ID, DEPOSIT_AMOUNT)
    assert response5.code == AccountManagerResponse.SUCCEED
    assert response5.response == INITIAL_BALANCE + DEPOSIT_AMOUNT
