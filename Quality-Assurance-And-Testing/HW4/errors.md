# Ошибка 1
1)  `return self.coins1`
2) Вызов метода `getCoins2` в начальном состоянии
3) Полученное: `AttributeError: 'VendingMachine' object has no attribute 'coins1'` 
Ожидаемое: `0`
4)  `return self.__coins1`

# Ошибка 2
1)  `def putCoin1(self):`
2) Вызов метода `putCoin1` в начальном состоянии
3) Полученное: `getCurrentBalance() == 2` 
Ожидаемое: `getCurrentBalance() == 1`
4)  `def putCoin1(self):` <--> `def putCoin2(self):`

# Ошибка 3
1)  `return self.__coins1`
2) Вызов метода `putCoin1` в начальном состоянии
3) Полученное: `getCoins2() == 1` 
Ожидаемое: `getCoins2() == 0`
4)  `return 0`

# Ошибка 4
1) -
2) Вызов метода `fillProducts` в начальном состоянии
3) Полученное: `OK` 
Ожидаемое: `ILLEGAL_OPERATION`
4)  `if self.__mode == VendingMachine.Mode.OPERATION:`
`return VendingMachine.Response.ILLEGAL_OPERATION`

# Ошибка 5
1) `return VendingMachine.Response.UNSUITABLE_CHANGE`
2) Вызов метода `enterAdminMode` при ненулевом балансе
3) Полученное: `UNSUITABLE_CHANGE` 
Ожидаемое: `CANNOT_PERFORM`
4)  `return VendingMachine.Response.CANNOT_PERFORM`

# Ошибка 6
1) `self.__num1 = self.__max2`
2) Вызов метода `fillProducts` в режиме отладки
3) Полученное: `getNumberOfProduct1() == 40` 
Ожидаемое: `getNumberOfProduct1() == 30`
4)  `self.__num1 = self.__max1`

# Ошибка 7
1) `if c1 <= 0 or c2 > self.__maxc2:`
2) Вызов `fillCoins(5, -5)` в режиме отладки
3) Полученное: `OK` 
Ожидаемое: `INVALID_PARAM`
4)  `if c2 <= 0 or c2 > self.__maxc2:`

# Ошибка 8
1) `if c1 <= 0 or c2 > self.__maxc1:`
2) Вызов `fillCoins(75, 5)` в режиме отладки
3) Полученное: `OK` 
Ожидаемое: `INVALID_PARAM`
4)  `if c1 <= 0 or c1 > self.__maxc1:`

# Ошибка 9
1) -
2) Вызов `setPrices(-5, 5)` в режиме отладки
3) Полученное: `OK` 
Ожидаемое: `INVALID_PARAM`
4)  `if p1 <= 0 or p2 <= 0:`
`return VendingMachine.Response.INVALID_PARAM`