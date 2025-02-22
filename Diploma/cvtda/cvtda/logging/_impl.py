from .cli import CLILogger

class LoggerContextManager():
    def __init__(self):
        self.__current_logger = CLILogger()

    def get(self):
        return self.__current_logger

    def set(self, new_current_logger):
        self.__current_logger = new_current_logger
