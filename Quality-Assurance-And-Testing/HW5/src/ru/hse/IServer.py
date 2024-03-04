import abc


class IServer(metaclass=abc.ABCMeta):
    @abc.abstractmethod
    def login(self, userName: str, mdPass: str):
        pass

    @abc.abstractmethod
    def logout(self, sess: int):
        pass

    @abc.abstractmethod
    def withdraw(self, sess: int, balance: float):
        pass

    @abc.abstractmethod
    def deposit(self, sess: int, balance: float):
        pass

    @abc.abstractmethod
    def getBalance(self, sess: int):
        pass
