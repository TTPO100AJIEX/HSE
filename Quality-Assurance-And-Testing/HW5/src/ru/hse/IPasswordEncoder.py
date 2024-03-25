import abc


class NullPointerException(Exception):
    pass


class IPasswordEncoder(metaclass=abc.ABCMeta):
    @abc.abstractmethod
    def makeSecure(self, password: str) -> str:
        pass
