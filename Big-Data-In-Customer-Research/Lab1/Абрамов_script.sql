/*
Запрос №1: Базовая фильтрация одной таблицы
	Вариант №2 Выберите все заказы со статусом ‘shipped’, созданные в 2017 году.
	Выведите order_id, order_status, order_purchase_timestamp и order_estimated_delivery_date
*/

SELECT id, status, purchase, estimated_delivery 
FROM olist.orders
WHERE EXTRACT('year' FROM purchase) == 2017 AND status = 'shipped';

/*
Запрос №2: Простое объединение двух таблиц
	Вариант №1 Соедините информацию о заказах и клиентах так, чтобы для
	каждого заказа из Сан-Паулу (customer_city = ‘S˜ao Paulo’) вывести:
	order_id, customer_city и customer_state.
*/

SELECT olist.orders.id AS order_id, olist.customers.city, olist.customers.state
FROM olist.orders
JOIN olist.customers ON olist.orders.customer_id = olist.customers.id
WHERE olist.customers.city == 'sao paulo';

/*
Запрос №3: Агрегация с группировкой и пост-фильтрацией
	Вариант №1 Для каждого штата (customer_state) рассчитайте общее количество
	заказов, сделанных в 2018 году. Оставьте в результате только те штаты,
	где было оформлено более 1000 заказов. Отсортируйте результат по убыванию числа заказов.
*/

SELECT olist.customers.state, COUNT(*) AS num_orders
FROM olist.orders
JOIN olist.customers ON olist.orders.customer_id = olist.customers.id
WHERE EXTRACT('year' FROM olist.orders.purchase) == 2018
GROUP BY olist.customers.state
HAVING COUNT(*) > 1000
ORDER BY num_orders DESC;

/*
Запрос №4: Многотабличное объединение (3+ таблицы)
	Вариант №2 Рассчитайте среднее время доставки (в днях) по каждому штату клиента
	(customer_state) и статусу заказа (order_status). Используйте таблицы orders, customers
	и рассчитайте разницу между order_delivered_customer_date и order_purchase_timestamp.
*/

SELECT
	olist.customers.state AS customer_state,
	olist.orders.status AS order_status,
	AVG(julian(olist.orders.delivered_customer) - julian(olist.orders.purchase)) AS mean_delivery_time
FROM olist.orders
JOIN olist.customers ON olist.orders.customer_id = olist.customers.id
GROUP BY (olist.customers.state, olist.orders.status)
ORDER BY (olist.customers.state, olist.orders.status);

/*
Запрос №5: Оконная функция (ранжирование внутри группы)
	Вариант №2 Для каждого клиента (customer_unique_id) пронумеруйте его заказы
	по хронологии (от самого раннего к самому позднему). Используйте оконную
	функцию ROW_NUMBER() с сортировкой по order_purchase_timestamp.
*/

SELECT
	olist.orders.id AS order_id,
	olist.customers.unique_id AS customer_unique_id,
	olist.orders.purchase AS purchase_timestamp,
	ROW_NUMBER() OVER (PARTITION BY olist.customers.unique_id ORDER BY olist.orders.purchase ASC) AS ranking
FROM olist.orders
JOIN olist.customers ON olist.orders.customer_id = olist.customers.id;

/*
Запрос №6*: OLAP-анализ с созданием новой метрики
	Вариант №1 Рассчитайте для каждого города клиента (customer_city)
	и статуса заказа (order_status) следующие метрики: среднее время доставки в днях
	(julianday(delivered) - julianday(purchase)); процент своевременных доставок
	(когда фактическая дата ≤ оценочной). Оставьте только те комбинации,
	где заказов ≥ 50. Отсортируйте по возрастанию среднего времени доставки.
*/

SELECT
	olist.customers.city AS customer_city,
	olist.orders.status AS order_status,
	AVG(julian(olist.orders.delivered_customer) - julian(olist.orders.purchase)) AS mean_delivery_time,
	100 * AVG(CAST(olist.orders.delivered_customer <= olist.orders.estimated_delivery AS SMALLINT)) AS on_time
FROM olist.orders
JOIN olist.customers ON olist.orders.customer_id = olist.customers.id
GROUP BY (olist.customers.city, olist.orders.status)
HAVING COUNT(*) > 50
ORDER BY mean_delivery_time ASC;

/*
Задание 2. Вариант №1
	1. На основе данных о заказах за 2018 год создайте метрику «частота заказов»
		для каждого клиента (customer_unique_id). Определите её как число заказов
		в месяц (общее число заказов / количество месяцев активности).
*/

/* В качестве месяцев активности будем считать месяцы, в которые клиент совершил хотя бы один заказ */

ALTER TABLE olist.customers ADD COLUMN order_frequency DOUBLE;

WITH order_frequencies AS (
	SELECT
		olist.customers.unique_id AS unique_id,
		COUNT(*) / COUNT(DISTINCT EXTRACT("month" FROM olist.orders.purchase)) AS order_frequency
	FROM olist.orders
	JOIN olist.customers ON olist.orders.customer_id = olist.customers.id
	WHERE EXTRACT('year' FROM olist.orders.purchase) == 2018
	GROUP BY olist.customers.unique_id
)
UPDATE olist.customers
SET order_frequency = order_frequencies.order_frequency
FROM order_frequencies
WHERE olist.customers.unique_id = order_frequencies.unique_id;

/*
	2. В таблице customers есть поле customer_state (штат Бразилии).
	Создайте новую колонку region в таблице customers, которая будет
	содержать географический регион по следующему правилу:
*/

/*
В идеале так. Но duckdb не умеет делать GENERATED в ALTER TABLE...

ALTER TABLE olist.customers 
ADD COLUMN  region VARCHAR GENERATED ALWAYS AS (
	CASE
        WHEN state IN ('SP', 'RJ', 'MG', 'ES') THEN 'Southeast'
        WHEN state IN ('RS', 'SC', 'PR') THEN 'South'
        WHEN state IN ('DF', 'GO', 'MT', 'MS') THEN 'Central-West'
        WHEN state IN ('BA', 'SE', 'AL', 'PE', 'PB', 'RN', 'CE', 'PI', 'MA') THEN 'Northeast'
        WHEN state IN ('AM', 'PA', 'RO', 'AC', 'RR', 'AP', 'TO') THEN 'North'
        ELSE NULL
	END
) VIRTUAL;

Поэтому только так:
*/

ALTER TABLE  olist.customers ADD COLUMN region VARCHAR;

UPDATE olist.customers
SET region = (
	CASE
        WHEN state IN ('SP', 'RJ', 'MG', 'ES') THEN 'Southeast'
        WHEN state IN ('RS', 'SC', 'PR') THEN 'South'
        WHEN state IN ('DF', 'GO', 'MT', 'MS') THEN 'Central-West'
        WHEN state IN ('BA', 'SE', 'AL', 'PE', 'PB', 'RN', 'CE', 'PI', 'MA') THEN 'Northeast'
        WHEN state IN ('AM', 'PA', 'RO', 'AC', 'RR', 'AP', 'TO') THEN 'North'
        ELSE NULL
	END
);

/* Проверка */
SELECT *
FROM olist.customers;
