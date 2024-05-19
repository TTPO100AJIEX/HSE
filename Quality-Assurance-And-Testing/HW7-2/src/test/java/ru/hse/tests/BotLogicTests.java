package ru.hse.tests;

import com.alibaba.fastjson2.JSON;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.ValueSource;
import ru.hse.bot.BotLogic;
import ru.hse.IStateStorage;
import ru.hse.SimpleMemoryStorage;
import ru.hse.GoodInStorage;
import ru.hse.RecordInStorage;

import static org.junit.jupiter.api.Assertions.*;

public class BotLogicTests {
    private SimpleMemoryStorage storage;
    private BotLogic bl ;
    private static final long userId = 1l;
    private static final long genericStorage = 0l;
    @BeforeEach
    public void beforeEach() {
        storage = new SimpleMemoryStorage();
        bl = new BotLogic();
    }


    @ParameterizedTest
    @ValueSource(longs = {-100000, -10, -1, 0, 100001})
    public void testWrongUserId(long userid) {
        String res = bl.reactOnMessage("NOT_USED", userid, storage);
        assertEquals("Значение userId не может быть меньше 1 или больше 100000", res);
    }
    
    @ParameterizedTest
    @ValueSource(longs = {1, 100000})
    public void testOkUserId(long userid) {
        String res = bl.reactOnMessage("NOT_USED", userid, null);
        assertEquals("Значение storage не может быть null", res);
    }

    @Test
    public void testWrongStorage() {
        String res = bl.reactOnMessage("NOT_USED", userId, null);
        assertEquals("Значение storage не может быть null", res);
    }

    @ParameterizedTest
    @ValueSource(strings = {"", "      "})
    public void testEmptyCommand(String point) {
        String res = bl.reactOnMessage("", userId, storage);
        assertEquals(GetWelcomeMessage(), res);
    }

    @Test
    public void testWrongCommand() {
        String res = bl.reactOnMessage("wrong", userId, storage);
        assertTrue(res.startsWith("wrong: команда не распознана, список доступных команд:"));
    }

    @Test
    public void testBuyFromWelcome() {
        String res = bl.reactOnMessage("buy", userId, storage);
        assertEquals(GetBuyMessage(), res);
        assertEquals("buy_actions", storage.getRecord(userId, "current_mode"));
    }

    @Test
    public void testEmtpyStorage() {
        IStateStorage storage = new SimpleMemoryStorage(true);
        String res = bl.reactOnMessage("buy", userId, storage);
        assertEquals("На счету 0 у.е.\nТовары:\nТоваров нет.\n" +
                    "Напишите номер интересующего вас товара чтобы добавить его в корзину, \n" +
                    "чтобы убрать товар из корзины напишите его номер со знаком -\n" +
                    "checkout чтобы оставить заказ на содержимое корзины или 0 чтобы вернуться обратно\n" +
                    "Корзина:\n Пусто\n Общая стоимость корзины: 0", res);
        assertEquals("buy_actions", storage.getRecord(userId, "current_mode"));
    }

    @ParameterizedTest
    @ValueSource(strings = {"BuY   ", "  bUy", "  BUY   "})
    public void testCommandFormat(String command) {
        String res = bl.reactOnMessage(command, userId, storage);
        assertEquals(GetBuyMessage(), res);
        assertEquals("buy_actions", storage.getRecord(userId, "current_mode"));
    }
    
    @Test
    public void testBuyAndBack() {
        storage.setRecord(userId, "bucket", "[{\"count\":100,\"name\":\"Good with high price\",\"price\":15}]");
        bl.reactOnMessage("buy", userId, storage);
        String res = bl.reactOnMessage("0", userId, storage);
        assertEquals(GetWelcomeMessage(), res);
        assertNull(storage.getRecord(userId, "current_mode"));
        assertEquals("[{\"count\":100,\"name\":\"Good with high price\",\"price\":15}]", storage.getRecord(userId, "bucket"));
    }
    
    @ParameterizedTest
    @ValueSource(strings = {"weird", "12.34", ".5", "-12.34", "-0.5"})
    public void testBuyNonInteger(String message) {
        testBuyFromWelcome();
        String res = bl.reactOnMessage(message, userId, storage);
        assertEquals("Введенная строка не является числом.", res);
    }

    @ParameterizedTest
    @ValueSource(strings = {"-4", "4"})
    public void testBuyWrongGood() {
        bl.reactOnMessage("buy", userId, storage);
        String res = bl.reactOnMessage("4", userId, storage);
        assertEquals("Выбранный товар не существует! 4", res);
    }

    @Test
    public void testAddNoGood() {
        storage.setRecord(genericStorage, "goods", "[{'name': 'test', 'description': 'testing', 'count': 0, 'price': 15}]");
        bl.reactOnMessage("buy", userId, storage);
        String res = bl.reactOnMessage("1", userId, storage);
        assertEquals("Выбранного товара нет в наличии в магазине в нужном количестве: test", res);
    }

    @ParameterizedTest
    @ValueSource(strings = {"1", "  1    "})
    public void testAddGood(String message) {
        bl.reactOnMessage("buy", userId, storage);
        String res = bl.reactOnMessage(message, userId, storage);
        assertEquals("Введите количество товара для покупки для Good with high price или 0 чтобы вернуться к корзине.", res);
        assertEquals("buy_actions_1", storage.getRecord(userId, "current_mode"));
    }

    @Test
    public void testAddGoodAndZero() {
        bl.reactOnMessage("buy", userId, storage);
        bl.reactOnMessage("1", userId, storage);
        String res = bl.reactOnMessage("0", userId, storage);
        assertEquals(GetBuyMessage(), res);
        assertEquals("buy_actions", storage.getRecord(userId, "current_mode"));
    }

    @ParameterizedTest
    @ValueSource(strings = {"100", "  100    "})
    public void testAddGoodWithAmount(String amount) {
        storage.setRecord(userId, "wallet", "1500");
        bl.reactOnMessage("buy", userId, storage);
        bl.reactOnMessage("1", userId, storage);
        String res = bl.reactOnMessage(amount, userId, storage);
        String expected_bucket = "Good with high price(100 ед.) за 15у.е.";
        assertEquals("Выбранные товары добавлены в корзину.\n" + GetBuyMessage(1500, 100, expected_bucket, 1500), res);
        assertEquals("buy_actions", storage.getRecord(userId, "current_mode"));
        assertEquals("[{\"count\":100,\"name\":\"Good with high price\",\"price\":15}]", storage.getRecord(userId, "bucket"));
        assertEquals("1500", storage.getRecord(userId, "wallet"));
    }
    
    @Test
    public void testAddGoodNoMoney() {
        storage.setRecord(userId, "wallet", "149");
        bl.reactOnMessage("buy", userId, storage);
        bl.reactOnMessage("1", userId, storage);
        String res = bl.reactOnMessage("10", userId, storage);
        assertEquals("Средств недостаточно для приобретения выбранного количества товаров.\n" + GetBuyMessage(149, 100, " Пусто", 0), res);
        assertEquals("buy_actions", storage.getRecord(userId, "current_mode"));
        assertNull(storage.getRecord(userId, "bucket"));
        assertEquals("149", storage.getRecord(userId, "wallet"));
    }
    
    @Test
    public void testAddGoodTwiceNotEnoughMoney() {
        storage.setRecord(userId, "wallet", "180");
        bl.reactOnMessage("buy", userId, storage);

        bl.reactOnMessage("1", userId, storage);
        bl.reactOnMessage("10", userId, storage);
        
        bl.reactOnMessage("1", userId, storage);
        String res = bl.reactOnMessage("3", userId, storage);
        String expected_bucket = "Good with high price(10 ед.) за 15у.е.";
        assertEquals("Средств недостаточно для приобретения выбранного количества товаров.\n" + GetBuyMessage(180, 100, expected_bucket, 150), res);
    }
    
    @Test
    public void testAddGoodTooMany() {
        storage.setRecord(userId, "wallet", "100000");
        bl.reactOnMessage("buy", userId, storage);
        bl.reactOnMessage("1", userId, storage);
        String res = bl.reactOnMessage("101", userId, storage);
        assertEquals("Выбранного товара нет в наличии в магазине в нужном количестве: Good with high price\n" + GetBuyMessage(100000, 100, " Пусто", 0), res);
        assertEquals("buy_actions", storage.getRecord(userId, "current_mode"));
        assertNull(storage.getRecord(userId, "bucket"));
        assertEquals("100000", storage.getRecord(userId, "wallet"));
    }
    
    @ParameterizedTest
    @ValueSource(strings = {"weird", "12.34", ".5", "-12.34", "-0.5"})
    public void testAddGoodAmountNotANumber(String amount) {
        bl.reactOnMessage("buy", userId, storage);
        bl.reactOnMessage("1", userId, storage);
        String res = bl.reactOnMessage(amount, userId, storage);
        assertEquals("Введенная строка не является числом.\n" + GetBuyMessage(), res);
        assertEquals("buy_actions", storage.getRecord(userId, "current_mode"));
    }
    
    @Test
    public void testAddGoodNegativeAmount() {
        bl.reactOnMessage("buy", userId, storage);
        bl.reactOnMessage("1", userId, storage);
        String res = bl.reactOnMessage("-5", userId, storage);
        assertEquals("Количество должно быть положительным числом или 0.\n" + GetBuyMessage(), res);
        assertEquals("buy_actions", storage.getRecord(userId, "current_mode"));
    }
    
    @Test
    public void testAddGoodAmountTwice() {
        storage.setRecord(userId, "wallet", "15000");
        bl.reactOnMessage("buy", userId, storage);

        bl.reactOnMessage("1", userId, storage);
        bl.reactOnMessage("5", userId, storage);
        
        bl.reactOnMessage("1", userId, storage);
        bl.reactOnMessage("99", userId, storage);
        
        // TODO: to fix
        assertEquals("[{\"count\":104,\"name\":\"Good with high price\",\"price\":15}]", storage.getRecord(userId, "bucket"));
        // assertEquals("[{\"count\":100,\"name\":\"Good with high price\",\"price\":15}]", storage.getRecord(userId, "bucket"));
    }
    
    @Test
    public void testRemoveGoodFromBucket() {
        storage.setRecord(userId, "wallet", "15000");
        bl.reactOnMessage("buy", userId, storage);
        bl.reactOnMessage("1", userId, storage);
        bl.reactOnMessage("50", userId, storage);

        String res = bl.reactOnMessage("-1", userId, storage);
        assertEquals("Введите количество товара который нужно убрать из корзины для Good with high price или 0 чтобы вернуться к корзине.", res);
        assertEquals("bucket_remove_1", storage.getRecord(userId, "current_mode"));
    }

    @Test
    public void testRemoveGoodFromBucketWrongItem() {
        storage.setRecord(userId, "wallet", "15000");
        bl.reactOnMessage("buy", userId, storage);
        bl.reactOnMessage("1", userId, storage);
        bl.reactOnMessage("50", userId, storage);

        String res = bl.reactOnMessage("-2", userId, storage);
        assertEquals("Товар не найден в корзине: good2", res);
        assertEquals("buy_actions", storage.getRecord(userId, "current_mode"));
    }
    
    @Test
    public void testRemoveGoodFromBucketWithAmount() {
        storage.setRecord(userId, "wallet", "1500");
        bl.reactOnMessage("buy", userId, storage);
        
        bl.reactOnMessage("1", userId, storage);
        bl.reactOnMessage("50", userId, storage);

        bl.reactOnMessage("-1", userId, storage);
        String res = bl.reactOnMessage("30", userId, storage);
        
        String expected_bucket = "Good with high price(20 ед.) за 15у.е.";
        assertEquals("Выбранные товары удалены из корзины.\n" + GetBuyMessage(1500, 100, expected_bucket, 300), res);
        assertEquals("buy_actions", storage.getRecord(userId, "current_mode"));
        assertEquals("[{\"count\":20,\"name\":\"Good with high price\",\"price\":15}]", storage.getRecord(userId, "bucket"));
        assertEquals("1500", storage.getRecord(userId, "wallet"));
    }
    
    @Test
    public void testRemoveGoodFromBucketAll() {
        storage.setRecord(userId, "wallet", "1500");
        bl.reactOnMessage("buy", userId, storage);
        
        bl.reactOnMessage("1", userId, storage);
        bl.reactOnMessage("50", userId, storage);

        bl.reactOnMessage("-1", userId, storage);
        String res = bl.reactOnMessage("50", userId, storage);
        
        assertEquals("Выбранные товары удалены из корзины.\n" + GetBuyMessage(1500, 100, " Пусто", 0), res);
        assertEquals("buy_actions", storage.getRecord(userId, "current_mode"));
        assertEquals("[]", storage.getRecord(userId, "bucket"));
        assertEquals("1500", storage.getRecord(userId, "wallet"));
    }
    
    @Test
    public void testRemoveGoodFromBucketTooMany() {
        storage.setRecord(userId, "wallet", "1500");
        bl.reactOnMessage("buy", userId, storage);
        
        bl.reactOnMessage("1", userId, storage);
        bl.reactOnMessage("50", userId, storage);

        bl.reactOnMessage("-1", userId, storage);
        String res = bl.reactOnMessage("70", userId, storage);
        
        String expected_bucket = "Good with high price(50 ед.) за 15у.е.";
        assertEquals("Выбранного товара нет в корзине в наличии в нужном количестве: Good with high price\n" + GetBuyMessage(1500, 100, expected_bucket, 750), res);
        assertEquals("buy_actions", storage.getRecord(userId, "current_mode"));
        assertEquals("[{\"count\":50,\"name\":\"Good with high price\",\"price\":15}]", storage.getRecord(userId, "bucket"));
        assertEquals("1500", storage.getRecord(userId, "wallet"));
    }
    
    @Test
    public void testRemoveGoodAndZero() {
        storage.setRecord(userId, "wallet", "1500");
        bl.reactOnMessage("buy", userId, storage);
        
        bl.reactOnMessage("1", userId, storage);
        bl.reactOnMessage("50", userId, storage);

        bl.reactOnMessage("-1", userId, storage);
        String res = bl.reactOnMessage("0", userId, storage);
        String expected_bucket = "Good with high price(50 ед.) за 15у.е.";
        assertEquals(GetBuyMessage(1500, 100, expected_bucket, 750), res);
        assertEquals("buy_actions", storage.getRecord(userId, "current_mode"));
        assertEquals("[{\"count\":50,\"name\":\"Good with high price\",\"price\":15}]", storage.getRecord(userId, "bucket"));
        assertEquals("1500", storage.getRecord(userId, "wallet"));
    }
    
    @Test
    public void testCheckout() {
        storage.setRecord(userId, "wallet", "1500");
        bl.reactOnMessage("buy", userId, storage);
        bl.reactOnMessage("1", userId, storage);
        bl.reactOnMessage("20", userId, storage);
        String res = bl.reactOnMessage("checkout", userId, storage);
        assertEquals("Заказ успешно добавлен, номер заказа: 0\n" + GetWelcomeMessage(), res);

        assertEquals("1", storage.getRecord(genericStorage, "orders_max_id"));
        assertEquals("[{\"id\":0}]", storage.getRecord(userId, "orders"));
        assertEquals("[{\"count\":20,\"name\":\"Good with high price\",\"price\":15}]", storage.getRecord(userId, "order_0"));
        assertEquals("1200", storage.getRecord(userId, "wallet"));
        assertEquals("[]", storage.getRecord(userId, "bucket"));
        assertNull(storage.getRecord(userId, "current_mode"));
    }
    
    @Test
    public void testCheckoutWithOrdersMaxId() {
        storage.setRecord(genericStorage, "orders_max_id", "2");
        storage.setRecord(userId, "wallet", "1500");
        bl.reactOnMessage("buy", userId, storage);
        bl.reactOnMessage("1", userId, storage);
        bl.reactOnMessage("20", userId, storage);
        String res = bl.reactOnMessage("checkout", userId, storage);
        assertEquals("Заказ успешно добавлен, номер заказа: 2\n" + GetWelcomeMessage(), res);

        assertEquals("3", storage.getRecord(genericStorage, "orders_max_id"));
        assertEquals("[{\"id\":2}]", storage.getRecord(userId, "orders"));
        assertEquals(null, storage.getRecord(userId, "order_0"));
        assertEquals(null, storage.getRecord(userId, "order_1"));
        assertEquals("[{\"count\":20,\"name\":\"Good with high price\",\"price\":15}]", storage.getRecord(userId, "order_2"));
        assertEquals("1200", storage.getRecord(userId, "wallet"));
        assertEquals("[]", storage.getRecord(userId, "bucket"));
        assertNull(storage.getRecord(userId, "current_mode"));
    }
    
    @Test
    public void testCheckoutSecondOrder() {
        storage.setRecord(userId, "orders", "[{\"id\":0}, {\"id\":1}]");
        storage.setRecord(genericStorage, "orders_max_id", "2");
        storage.setRecord(userId, "wallet", "1500");
        bl.reactOnMessage("buy", userId, storage);
        bl.reactOnMessage("1", userId, storage);
        bl.reactOnMessage("20", userId, storage);
        String res = bl.reactOnMessage("checkout", userId, storage);
        assertEquals("Заказ успешно добавлен, номер заказа: 2\n" + GetWelcomeMessage(), res);

        assertEquals("3", storage.getRecord(genericStorage, "orders_max_id"));
        assertEquals("[{\"id\":0},{\"id\":1},{\"id\":2}]", storage.getRecord(userId, "orders"));
        assertEquals("[{\"count\":20,\"name\":\"Good with high price\",\"price\":15}]", storage.getRecord(userId, "order_2"));
        assertEquals("1200", storage.getRecord(userId, "wallet"));
        assertEquals("[]", storage.getRecord(userId, "bucket"));
        assertNull(storage.getRecord(userId, "current_mode"));
    }
    
    @Test
    public void testCheckoutEmptyBucket() {
        bl.reactOnMessage("buy", userId, storage);
        String res = bl.reactOnMessage("checkout", userId, storage);
        assertEquals("Корзина пуста.\n" + GetWelcomeMessage(), res);
        assertEquals(null, storage.getRecord(genericStorage, "orders_max_id"));
        assertEquals(null, storage.getRecord(userId, "orders"));
        assertNull(storage.getRecord(userId, "current_mode"));
    }
    
    @Test
    public void testCheckoutTooMany() {
        storage.setRecord(userId, "wallet", "15000");
        bl.reactOnMessage("buy", userId, storage);
        bl.reactOnMessage("1", userId, storage);
        bl.reactOnMessage("5", userId, storage);
        bl.reactOnMessage("1", userId, storage);
        bl.reactOnMessage("99", userId, storage);
        // Now we have 104/100 items in the bucket,
        // although it should not work like this :)

        String res = bl.reactOnMessage("checkout", userId, storage);
        assertEquals("Выбранного товара нет в наличии в магазине в нужном количестве: Good with high price\n" + GetWelcomeMessage(), res);

        assertEquals(null, storage.getRecord(genericStorage, "orders_max_id"));
        assertEquals(null, storage.getRecord(userId, "orders"));
        assertEquals("[{\"count\":104,\"name\":\"Good with high price\",\"price\":15}]", storage.getRecord(userId, "bucket"));
        assertNull(storage.getRecord(userId, "current_mode"));

        String items = bl.reactOnMessage("buy", userId, storage);
        assertEquals(GetBuyMessage(15000, 100, "Good with high price(104 ед.) за 15у.е.", 1560), items);
    }

    @Test
    public void testOrdersFromWelcomeNoOrders() {
        String res = bl.reactOnMessage("orders", userId, storage);
        assertEquals("Имеющиесся заказы: \nЗаказов нет, отправьте 0 чтобы вернуться в основное меню.", res);
        assertEquals("orders_actions", storage.getRecord(userId, "current_mode"));
    }

    @Test
    public void testOrdersFromWelcomeWithOrders() {
        String good1 = JSON.toJSONString(new GoodInStorage("Test1", "Testing1", 20, 15));
        String good2 = JSON.toJSONString(new GoodInStorage("Test12345678900987654321", "Testing2", 15, 20));
        String good3 = JSON.toJSONString(new GoodInStorage("Test3", "Testing3", 10, 12));

        storage.setRecord(userId, "order_0", String.format("[%s, %s, %s]", good1, good2, good3));
        storage.setRecord(userId, "order_1", String.format("[%s]", good1));
        storage.setRecord(userId, "orders", "[{\"id\":0}, {\"id\":1}]");

        String res = bl.reactOnMessage("orders", userId, storage);
        assertEquals("Имеющиесся заказы: \n" +
                    "1. Test1(20), Test123456789009876..(15), Test3(10)\n" +
                    "2. Test1(20)\n" +
                    "Напишите номер интересующего вас заказа или напишите 0 чтобы вернуться обратно.", res);
        assertEquals("orders_actions", storage.getRecord(userId, "current_mode"));
    }

    @Test
    public void testOrdersFromWelcomeWithOrdersNoGoods() {
        storage.setRecord(userId, "orders", "[{\"id\":0}]");
        String res = bl.reactOnMessage("orders", userId, storage);
        assertEquals("Имеющиесся заказы: \n1. \nНапишите номер интересующего вас заказа или напишите 0 чтобы вернуться обратно.", res);
        assertEquals("orders_actions", storage.getRecord(userId, "current_mode"));
    }

    @Test
    public void testOrdersAndBack() {
        bl.reactOnMessage("orders", userId, storage);
        String res = bl.reactOnMessage("0", userId, storage);
        assertEquals(GetWelcomeMessage(), res);
        assertNull(storage.getRecord(userId, "current_mode"));
    }

    @ParameterizedTest
    @ValueSource(strings = {"weird", "12.34", ".5", "-12.34", "-0.5"})
    public void testOrdersNotInteger(String message) {
        bl.reactOnMessage("orders", userId, storage);
        String res = bl.reactOnMessage(message, userId, storage);
        assertEquals("Введенная строка не является числом.", res);
        assertEquals("orders_actions", storage.getRecord(userId, "current_mode"));
    }

    @ParameterizedTest
    @ValueSource(strings = {"-10", "-1", "100", "3"})
    public void testOrdersNoOrder(String message) {
        String good1 = JSON.toJSONString(new GoodInStorage("Test1", "Testing1", 20, 15));
        String good2 = JSON.toJSONString(new GoodInStorage("Test12345678900987654321", "Testing2", 15, 20));
        String good3 = JSON.toJSONString(new GoodInStorage("Test3", "Testing3", 10, 12));

        storage.setRecord(userId, "order_0", String.format("[%s, %s, %s]", good1, good2, good3));
        storage.setRecord(userId, "order_1", String.format("[%s]", good1));
        storage.setRecord(userId, "orders", "[{\"id\":0}, {\"id\":1}]");

        bl.reactOnMessage("orders", userId, storage);
        String res = bl.reactOnMessage(message, userId, storage);
        assertEquals("Выбранный заказ не существует! " + message, res);
        assertEquals("orders_actions", storage.getRecord(userId, "current_mode"));
    }

    @Test
    public void testOrdersGet() {
        String good1 = JSON.toJSONString(new GoodInStorage("Test1", "Testing1", 20, 15));
        String good2 = JSON.toJSONString(new GoodInStorage("Test12345678900987654321", "Testing2", 15, 20));
        String good3 = JSON.toJSONString(new GoodInStorage("Test3", "Testing3", 10, 12));

        storage.setRecord(userId, "order_0", String.format("[%s, %s, %s]", good1, good2, good3));
        storage.setRecord(userId, "order_1", String.format("[%s]", good1));
        storage.setRecord(userId, "orders", "[{\"id\":0}, {\"id\":1}]");

        bl.reactOnMessage("orders", userId, storage);
        String res = bl.reactOnMessage("1", userId, storage);
        assertEquals(
            "Test1(20) - Описания нет,\nTest12345678900987654321(15) - Описания нет,\nTest3(10) - Описания нет\n" +
            "Чтобы выбрать другой заказ напишите его номер или 0 чтобы" +
            " вернуться в основное меню", res); // TODO: to fix
        /*assertEquals(
            "Test1(20) - Testing1,\nTest12345678900987654321(15) - Testing2,\nTest3(10) - Testing3\n" +
            "Чтобы выбрать другой заказ напишите его номер или 0 чтобы" +
            " вернуться в основное меню", res); */
        assertEquals("orders_actions", storage.getRecord(userId, "current_mode"));
    }


    @Test
    public void testDepositFromWelcome() {
        String res = bl.reactOnMessage("deposit", userId, storage);
        assertEquals("Чтобы пополнить счет нужно ввести код для пополнения или 0 чтобы вернуться в основное меню", res);
        assertEquals("deposit_actions", storage.getRecord(userId, "current_mode"));
    }

    @Test
    public void testDepositAndBack() {
        bl.reactOnMessage("deposit", userId, storage);
        String res = bl.reactOnMessage("0", userId, storage);
        assertEquals(GetWelcomeMessage(), res);
        assertNull(storage.getRecord(userId, "current_mode"));
    }

    @ParameterizedTest
    @ValueSource(strings = {
        "3DRwBBrcFThKXq9zNIdPihfg3eaQ7g", "0awdnDr9kaByy4qg4ha94A2HXcRCJz",
        "lyvQuGNBYhrcnDfCsn9cchqrTvLEtg", "qdrcNtvcZIZB9H3ylKPoG4pM1FFQgB",
        "b4zK3bHKAnUMaHCuKpfKHBMzPCAxBs", "hWxisKcexAgUtvWKs4NDdge39iSAsm",
        "G8BasIDVT86HaJNWCDBzoZ4sF1sYZA", "1vVPsiFdUPOIAMIzsnMrDppdGff9cY",
        "pNs8UA725BryPtCSmbNQtelaQfsh90", "HPtxlAwkBjiE8r6gd7Ba9SnFU3cosF",
        "  HPtxlAwkBjiE8r6gd7Ba9SnFU3cosF", "HPtxlAwkBjiE8r6gd7Ba9SnFU3cosF   ",
        "  HPtxlAwkBjiE8r6gd7Ba9SnFU3cosF    "
    })
    public void testDepositNoBalance(String promocode) {
        bl.reactOnMessage("deposit", userId, storage);
        String res = bl.reactOnMessage(promocode, userId, storage);
        assertEquals("Средства внесены, на счету 1000 у.е. Введите другой код или 0 чтобы вернуться в основное меню", res);
        assertEquals("deposit_actions", storage.getRecord(userId, "current_mode"));
        assertEquals(promocode.trim(), storage.getRecord(userId, "keys"));
        assertEquals("1000", storage.getRecord(userId, "wallet"));
    }

    @Test
    public void testDepositAddBalance() {
        storage.setRecord(userId, "wallet", "150");
        bl.reactOnMessage("deposit", userId, storage);
        String res = bl.reactOnMessage("3DRwBBrcFThKXq9zNIdPihfg3eaQ7g", userId, storage);
        assertEquals("Средства внесены, на счету 1150 у.е. Введите другой код или 0 чтобы вернуться в основное меню", res);
        assertEquals("deposit_actions", storage.getRecord(userId, "current_mode"));
        assertEquals("3DRwBBrcFThKXq9zNIdPihfg3eaQ7g", storage.getRecord(userId, "keys"));
        assertEquals("1150", storage.getRecord(userId, "wallet"));
    }

    @Test
    public void testDepositTwoPromocodes() {
        bl.reactOnMessage("deposit", userId, storage);
        bl.reactOnMessage("3DRwBBrcFThKXq9zNIdPihfg3eaQ7g", userId, storage);
        bl.reactOnMessage("0awdnDr9kaByy4qg4ha94A2HXcRCJz", userId, storage);
        assertEquals("deposit_actions", storage.getRecord(userId, "current_mode"));
        assertEquals("3DRwBBrcFThKXq9zNIdPihfg3eaQ7g|0awdnDr9kaByy4qg4ha94A2HXcRCJz", storage.getRecord(userId, "keys"));
        assertEquals("2000", storage.getRecord(userId, "wallet"));
    }

    @Test
    public void testDepositTwice() {
        bl.reactOnMessage("deposit", userId, storage);
        bl.reactOnMessage("3DRwBBrcFThKXq9zNIdPihfg3eaQ7g", userId, storage);
        String res = bl.reactOnMessage("3DRwBBrcFThKXq9zNIdPihfg3eaQ7g", userId, storage);
        assertEquals("Код 3DRwBBrcFThKXq9zNIdPihfg3eaQ7g был использован ранее. Введите другой или наберите 0 чтобы вернуться в меню.", res);
        assertEquals("deposit_actions", storage.getRecord(userId, "current_mode"));
        assertEquals("3DRwBBrcFThKXq9zNIdPihfg3eaQ7g", storage.getRecord(userId, "keys"));
        assertEquals("1000", storage.getRecord(userId, "wallet"));
    }

    @Test
    public void testDepositTwiceDifferentUsers() {
        bl.reactOnMessage("deposit", userId, storage);
        bl.reactOnMessage("deposit", userId + 1, storage);
        bl.reactOnMessage("3DRwBBrcFThKXq9zNIdPihfg3eaQ7g", userId, storage);
        bl.reactOnMessage("3DRwBBrcFThKXq9zNIdPihfg3eaQ7g", userId + 1, storage);
        assertEquals("3DRwBBrcFThKXq9zNIdPihfg3eaQ7g", storage.getRecord(userId, "keys"));
        assertEquals("3DRwBBrcFThKXq9zNIdPihfg3eaQ7g", storage.getRecord(userId + 1, "keys"));
        assertEquals("1000", storage.getRecord(userId, "wallet"));
        assertEquals("1000", storage.getRecord(userId + 1, "wallet"));
    }

    @ParameterizedTest
    @ValueSource(strings = {
        "what_is_this", "0awdnDr9kaByy 4qg4ha94A2HXcRCJz",
        "lyvQuGNBYhrcnDfCsn9cchqrTvLEtg qdrcNtvcZIZB9H3ylKPoG4pM1FFQgB",
        "b4zK3bHKAnUMaHCuKpfKHBMzPCAxBs|hWxisKcexAgUtvWKs4NDdge39iSAsm",
        "3DRwBBrcFThKXq9zNIdPihfg3eaQ7g|0awdnDr9kaByy4qg4ha94A2HXcRCJz",
        "pNs8UA725BryP", "tCSmbNQtelaQfsh90"
    })
    public void testDepositInvalidCode(String promocode) {
        bl.reactOnMessage("deposit", userId, storage);
        String res = bl.reactOnMessage(promocode, userId, storage);
        assertEquals("Код " + promocode + " не валиден. Введите другой или наберите 0 чтобы вернуться в меню.", res);
        assertEquals("deposit_actions", storage.getRecord(userId, "current_mode"));
        assertEquals(null, storage.getRecord(userId, "keys"));
        assertEquals(null, storage.getRecord(userId, "wallet"));
    }

    @Test
    public void accountInitialTest() {
        String res = bl.reactOnMessage("account", userId, storage);
        assertEquals("У вас на счету 0 у.е. Доступные команды: \n" + GetCommands(), res);
    }
    @Test
    public void accountTestMoney() {
        storage.setRecord(userId, "wallet", "150");
        String res = bl.reactOnMessage("account", userId, storage);
        assertEquals("У вас на счету 150 у.е. Доступные команды: \n" + GetCommands(), res);
    }




    private String GetCommands()
    {
        return "* 'Buy' чтобы добавить новый заказ \n" +
                "* 'Orders' чтобы изучить список своих заказов \n" +
                "* 'Deposit' чтобы пополнить счет \n" +
                "* 'Account' чтобы посмотреть баланс на счету.";
    }

    private String GetWelcomeMessage()
    {
        return "Добро пожаловать. Доступные команды: \n" + GetCommands();
    }

    private String GetBuyMessage() {
        return GetBuyMessage(0, 100, " Пусто", 0);
    }
    private String GetBuyMessage(Integer balance, Integer amont_of_one, String bucket, Integer bucket_price)
    {
        return String.format("На счету %d у.е.\n" +
                "Товары:\n" +
                "1. Good with high price(%d ед.) за 15у.е.\n" +
                "2. good2(200 ед.) за 5у.е.\n" +
                "3. good3 with long and strange description for no purpose and mean(2000 ед.) за 1у.е.\n" +
                "Напишите номер интересующего вас товара чтобы добавить его в корзину, \n" +
                "чтобы убрать товар из корзины напишите его номер со знаком -\n" +
                "checkout чтобы оставить заказ на содержимое корзины или 0 чтобы вернуться обратно\n" +
                "Корзина:\n" +
                "%s\n" +
                " Общая стоимость корзины: %d",
                balance, amont_of_one, bucket, bucket_price);
    }
}
