from ru.hse.ServerResponse import ServerResponse
from ru.hse.AccountManagerResponse import AccountManagerResponse  

def test_scenario_2(account_manager, mock_password_encoder, mock_server):
    CORRECT_PASSWORD = "correct-password"
    CORRECT_PASSWORD_HASH = "correct_password_hash"
    CORRECT_LOGIN = "correct-login"
    CORRECT_SESSION_ID = 12345
    WRONG_SESSION_ID = 23456

    INITIAL_BALANCE = 10.0
    WITHDRAW_AMOUNT = 50.0
    DEPOSIT_AMOUNT = 100.0

    mock_password_encoder("makeSecure", ( CORRECT_PASSWORD, ), CORRECT_PASSWORD_HASH)
    mock_server("login", ( CORRECT_LOGIN, CORRECT_PASSWORD_HASH ), ServerResponse.SUCCESS, CORRECT_SESSION_ID)

    # Пользователь (user) проводит успешную авторизацию
    response1 = account_manager.callLogin(CORRECT_LOGIN, CORRECT_PASSWORD)
    assert response1.code == AccountManagerResponse.SUCCEED
    assert response1.response == CORRECT_SESSION_ID

    # Проводится попытка снятия 50 единиц (неудачная)
    CURRENT_BALANCE = INITIAL_BALANCE
    mock_server("withdraw", ( CORRECT_SESSION_ID, WITHDRAW_AMOUNT ), ServerResponse.NO_MONEY, CURRENT_BALANCE)
    response2 = account_manager.withdraw(CORRECT_LOGIN, CORRECT_SESSION_ID, WITHDRAW_AMOUNT)
    assert response2.code == AccountManagerResponse.NO_MONEY
    assert response2.response == CURRENT_BALANCE

    # Делается запрос на количество средств на счету.
    mock_server("getBalance", ( CORRECT_SESSION_ID, ), ServerResponse.SUCCESS, CURRENT_BALANCE)
    response3 = account_manager.getBalance(CORRECT_LOGIN, CORRECT_SESSION_ID)
    assert response3.code == AccountManagerResponse.SUCCEED
    assert response3.response == CURRENT_BALANCE
    
    # Проводится внесение 100 единиц
    CURRENT_BALANCE += DEPOSIT_AMOUNT
    mock_server("deposit", ( CORRECT_SESSION_ID, DEPOSIT_AMOUNT ), ServerResponse.SUCCESS, CURRENT_BALANCE)
    response4 = account_manager.deposit(CORRECT_LOGIN, CORRECT_SESSION_ID, DEPOSIT_AMOUNT)
    assert response4.code == AccountManagerResponse.SUCCEED
    assert response4.response == CURRENT_BALANCE

    # Проводится снятие 50 единиц с указанием некорректного номера сессии (неудачное).
    response5 = account_manager.withdraw(CORRECT_LOGIN, WRONG_SESSION_ID, WITHDRAW_AMOUNT)
    assert response5.code == AccountManagerResponse.INCORRECT_SESSION
    assert response5.response is None

    # Проводится снятие 50 единиц с правильным номером сессии.
    CURRENT_BALANCE -= WITHDRAW_AMOUNT
    mock_server("withdraw", ( CORRECT_SESSION_ID, WITHDRAW_AMOUNT ), ServerResponse.SUCCESS, CURRENT_BALANCE)
    response6 = account_manager.withdraw(CORRECT_LOGIN, CORRECT_SESSION_ID, WITHDRAW_AMOUNT)
    assert response6.code == AccountManagerResponse.SUCCEED
    assert response6.response == CURRENT_BALANCE

    # Проводится выход из системы (logout).
    mock_server("logout", ( CORRECT_SESSION_ID, ), ServerResponse.SUCCESS, None)
    response7 = account_manager.callLogout(CORRECT_LOGIN, CORRECT_SESSION_ID)
    assert response7.code == AccountManagerResponse.SUCCEED
    assert response7.response is None