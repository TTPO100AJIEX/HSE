# Ошибка 1
1)  `return self.coins1`
2) Вызов метода `getCoins2` в начальном состоянии
3) Полученное: `AttributeError: 'VendingMachine' object has no attribute 'coins1'` 
Ожидаемое: `0`
4)  `return self.__coins1`
