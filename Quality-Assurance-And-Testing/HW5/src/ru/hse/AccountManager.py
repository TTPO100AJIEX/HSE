from ru.hse import IServer, IPasswordEncoder, AccountManagerResponse, ServerResponse
from ru.hse.AccountManagerResponse import AccountManagerResponse
from ru.hse.ServerResponse import ServerResponse


class AccountManager:
    server: IServer
    activeAccounts = dict[str, int]()
    passEncoder: IPasswordEncoder

    def __init__(self, s: IServer, encoder: IPasswordEncoder):
        self.server = s

    def callLogin(self, login: str, password: str) -> AccountManagerResponse:
        sess: int = self.activeAccounts.get(login, None)
        if sess is not None:
            return AccountManagerResponse.ACCOUNT_MANAGER_RESPONSE
        ret: ServerResponse = self.server.login(login, self.passEncoder.makeSecure(password))
        match ret.code:
            case ServerResponse.ALREADY_LOGGED:
                return AccountManagerResponse.ACCOUNT_MANAGER_RESPONSE
            case ServerResponse.NO_USER_INCORRECT_PASSWORD:
                return AccountManagerResponse.NO_USER_INCORRECT_PASSWORD_RESPONSE
            case ServerResponse.SUCCESS:
                resp = ret.response
                if isinstance(resp, int):
                    return AccountManagerResponse(AccountManagerResponse.SUCCEED, resp)
        return AccountManagerResponse(AccountManagerResponse.INCORRECT_RESPONSE, ret)

    def callLogout(self, user: str, session: int) -> AccountManagerResponse:
        rem: int = self.activeAccounts.pop(user, None)
        if rem is None:
            return AccountManagerResponse.NOT_LOGGED_RESPONSE
        resp: ServerResponse = self.server.logout(session)
        match resp.code:
            case ServerResponse.NOT_LOGGED:
                return AccountManagerResponse.NOT_LOGGED_RESPONSE
            case ServerResponse.SUCCESS:
                return AccountManagerResponse.SUCCEED_RESPONSE
        return AccountManagerResponse(AccountManagerResponse.INCORRECT_RESPONSE, resp)

    def withdraw(self, user: str, session: int, amount: float):
        stored: int = self.activeAccounts.get(user, None)
        if stored is None:
            return AccountManagerResponse.NOT_LOGGED_RESPONSE
        if stored != session:
            return AccountManagerResponse.INCORRECT_SESSION_RESPONSE
        resp: ServerResponse = self.server.withdraw(session, amount)
        match resp.code:
            case ServerResponse.NOT_LOGGED:
                return AccountManagerResponse.NOT_LOGGED_RESPONSE;
            case ServerResponse.NO_MONEY:
                r = resp.response
                if r is not None and (isinstance(r, float)):
                    return AccountManagerResponse(AccountManagerResponse.NO_MONEY, r)
            case ServerResponse.SUCCESS:
                r = resp.response
                if r is not None and (isinstance(r, float)):
                    return AccountManagerResponse(AccountManagerResponse.SUCCEED, r)
        return AccountManagerResponse(AccountManagerResponse.INCORRECT_RESPONSE, resp)

    def deposit(self, user: str, session: int, amount: float) -> AccountManagerResponse:
        stored: int = self.activeAccounts.get(user, None)
        if stored is None:
            return AccountManagerResponse.NOT_LOGGED_RESPONSE
        if stored != session:
            return AccountManagerResponse.INCORRECT_SESSION_RESPONSE
        resp: ServerResponse = self.server.deposit(session, amount)
        match resp.code:
            case ServerResponse.NOT_LOGGED:
                return AccountManagerResponse.NOT_LOGGED_RESPONSE
            case ServerResponse.NO_MONEY:
                r = resp.response
                if r is not None and (isinstance(r, float)):
                    return AccountManagerResponse(AccountManagerResponse.NO_MONEY, r)
            case ServerResponse.SUCCESS:
                r = resp.response
                if r is not None and (isinstance(r, float)):
                    return AccountManagerResponse(AccountManagerResponse.SUCCEED, r)
        return AccountManagerResponse(AccountManagerResponse.INCORRECT_RESPONSE, resp)

    def getBalance(self, user: str, session: int) -> AccountManagerResponse:
        stored: int = self.activeAccounts.get(user, None)
        if stored is None:
            return AccountManagerResponse.NOT_LOGGED_RESPONSE
        if stored != session:
            return AccountManagerResponse.INCORRECT_SESSION_RESPONSE
        resp: ServerResponse = self.server.getBalance(session)
        match resp.code:
            case ServerResponse.NOT_LOGGED:
                return AccountManagerResponse.NOT_LOGGED_RESPONSE
            case ServerResponse.SUCCESS:
                r = resp.response
                if r is not None and (isinstance(r, float)):
                    return AccountManagerResponse(AccountManagerResponse.SUCCEED, r)
        return AccountManagerResponse(AccountManagerResponse.INCORRECT_RESPONSE, resp)
