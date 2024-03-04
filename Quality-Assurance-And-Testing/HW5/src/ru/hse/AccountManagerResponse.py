class AccountManagerResponse:
    SUCCEED = 0
    ALREADY_LOGGED = 1
    NOT_LOGGED = 2
    NO_USER_INCORRECT_PASSWORD = 3
    INCORRECT_RESPONSE = 4
    UNDEFINED_ERROR = 5
    INCORRECT_SESSION = 6
    NO_MONEY = 7
    ENCODING_ERROR = 8
    code: int
    response = None

    def __init__(self, code: int, obj):
        self.code = code
        self.response = obj


ACCOUNT_MANAGER_RESPONSE = AccountManagerResponse(AccountManagerResponse.ALREADY_LOGGED, None)
NO_USER_INCORRECT_PASSWORD_RESPONSE = AccountManagerResponse(AccountManagerResponse.NO_USER_INCORRECT_PASSWORD, None)
UNDEFINED_ERROR_RESPONSE = AccountManagerResponse(AccountManagerResponse.UNDEFINED_ERROR, None)
NOT_LOGGED_RESPONSE = AccountManagerResponse(AccountManagerResponse.NOT_LOGGED, None)
INCORRECT_SESSION_RESPONSE = AccountManagerResponse(AccountManagerResponse.INCORRECT_SESSION, None)
SUCCEED_RESPONSE = AccountManagerResponse(AccountManagerResponse.SUCCEED, None)
NO_MONEY_RESPONSE = AccountManagerResponse(AccountManagerResponse.NO_MONEY, None)
ENCODING_ERROR_RESPONSE = AccountManagerResponse(AccountManagerResponse.ENCODING_ERROR, None)
