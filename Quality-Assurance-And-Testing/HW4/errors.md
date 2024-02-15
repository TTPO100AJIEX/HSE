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

# Ошибка 10
1) `self.__coins1 -= self.__balance // self.__coinval2`
`self.__coins2 -= 1`
2) Вызов `returnMoney`, когда автомат находится в рабочем режиме с 21 монетой 1 типа и 12 монетами 2 типа, баланс - 13.
3) Полученное: после перехода в режим отладки `getCoins1() == 15`, `getCoins2() == 11`
Ожидаемое: после перехода в режим отладки `getCoins1() == 20`, `getCoins2() == 6`
4)  `self.__coins2 -= self.__balance // self.__coinval2`
`self.__coins1 -= 1`

# Ошибка 11
1) `if number <= 0 or number >= self.__max1:`
2) Вызов `giveProduct1(30)`, когда автомат в рабочем режиме с максимальным количеством продуктов, но без монет
3) Полученное: `INVALID_PARAM`
Ожидаемое: `INSUFFICIENT_MONEY`
4)  `if number <= 0 or number > self.__max1:`

# Ошибка 12
1) `self.__coins1 -= res // self.__coinval2`
`self.__coins2 -= 1`
2) Вызов `giveProduct2(2)`, когда автомат в рабочем режиме с 32 монетами 1 типа, 6 монетами 2 типа, балансом 27 при цене предмета типа 2, равной 8
3) Полученное: после перехода в режим отладки `getCoins1() == 27`, `getCoins2() == 5`
Ожидаемое: после перехода в режим отладки `getCoins1() == 31`, `getCoins2() == 1`
4)  `self.__coins2 -= res // self.__coinval2`
`self.__coins1 -= 1`

# Ошибка 13
1) `if number <= 0 or number >= self.__max2:`
2) Вызов `giveProduct2(40)`, когда автомат в рабочем режиме с максимальным количеством продуктов, но без монет
3) Полученное: `INVALID_PARAM`
Ожидаемое: `INSUFFICIENT_MONEY`
4)  `if number <= 0 or number > self.__max2:`

# Ошибка 14
1) `if res > self.__coins1 * self.__coinval1 + self.__coins2 * self.__coinval2:`
`return VendingMachine.Response.INSUFFICIENT_MONEY`
2, 3) Методом "серой коробки" не воспроизводится
4)  `if res > self.__coins1 * self.__coinval1 + self.__coins2 * self.__coinval2:`
`return VendingMachine.Response.TOO_BIG_CHANGE`