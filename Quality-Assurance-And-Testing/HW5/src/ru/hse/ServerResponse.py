class ServerResponse:
	SUCCESS = 0
	UNDEFINED_ERROR = 1
	ALREADY_LOGGED = 2
	NOT_LOGGED = 3
	NO_USER_INCORRECT_PASSWORD = 4
	NO_MONEY = 5
	code: int
	response = None
	
	def __init__(self, code: int, obj):
		self.code = code
		self.response = obj
