package ru.hse.bot;

import com.alibaba.fastjson2.JSON;
import ru.hse.GoodInStorage;
import ru.hse.IStateStorage;
import ru.hse.RecordInStorage;

import java.util.*;

public class BotLogic {
    private static final long GENERIC_STORAGE = 0l;
    private static final String EMPTY_BUCKET = "Корзина пуста.";
    private static final String THANKS_BUCKET = "Спасибо за покупку! Средства списаны со счета, товары скоро будут к вам отправлены";
    private static final String GOOD_NOT_EXIST = "Выбранный товар не существует!";
    private static final String ORDER_NOT_EXIST = "Выбранный заказ не существует!";
    private static final String NOT_A_NUMBER = "Введенная строка не является числом.";
    private static final String NUMBER_MUST_BE_POSITIVE = "Количество должно быть положительным числом или 0.";
    private static final String NO_GOOD_IN_STORE = "Выбранного товара нет в наличии в магазине в нужном количестве: ";
    private static final String NO_GOOD_IN_BUCKET = "Выбранного товара нет в корзине в наличии в нужном количестве: ";
    private static final String NO_MONEY = "Средств недостаточно для приобретения выбранного количества товаров.";
    private static final String GOODS_ADDED = "Выбранные товары добавлены в корзину.";
    private static final String GOODS_BUCKET_REMOVED = "Выбранные товары удалены из корзины.";
    private static final String CHECK_MORE_GOODS = "Чтобы выбрать другой товар напишите его номер или 0 чтобы" +
            " вернуться в основное меню";
    private static final String CHECK_MORE_ORDERS = "Чтобы выбрать другой заказ напишите его номер или 0 чтобы" +
            " вернуться в основное меню";

    private static final String WRONG_USER_ID = "Значение userId не может быть меньше 1 или больше 100000";
    private static final String WRONG_STORAGE = "Значение storage не может быть null";
    private static final String REMOVE_BUCKET_COUNT_FOR_GOOD = "Введите количество товара который нужно убрать из корзины для ";
    private static final String BUCKET_NOT_FOUND = "Товар не найден в корзине: ";
    private static final String SELECT_COUNT_FOR_GOOD = "Введите количество товара для покупки для ";
    private static final String COMMAND_NOT_FOUND = ": команда не распознана, список доступных команд:";
    private  static final String COMMANDS =
            "* 'Buy' чтобы добавить новый заказ \n" +
                    "* 'Orders' чтобы изучить список своих заказов \n" +
                    "* 'Deposit' чтобы пополнить счет \n" +
                    "* 'Account' чтобы посмотреть баланс на счету.";
    private static final String INITIAL_INFO="Добро пожаловать. Доступные команды: \n" +
            COMMANDS;

    private static final String BUCKET_MESSAGE = "Напишите номер интересующего вас товара чтобы добавить его в корзину, \n" +
            "чтобы убрать товар из корзины напишите его номер со знаком -\n"+
            "checkout чтобы оставить заказ на содержимое корзины или 0 чтобы вернуться обратно";

    public final String reactOnMessage(String message, long userid, IStateStorage storage) {
        if(userid<1 || userid>100000)
            return WRONG_USER_ID;
        if(storage == null)
            return WRONG_STORAGE;
        String mode = storage.getRecord(userid, "current_mode");
        message = message.trim();
        if(mode == null) {
            if(message.isEmpty())
              return INITIAL_INFO;
            String ret = handleNullMode(message, userid, storage);
            if(ret == null)
                ret = message + COMMAND_NOT_FOUND+"\n" + COMMANDS;
            return ret;
        }
        mode = mode.toLowerCase();
        switch (mode){
            case "buy_actions":
                if(message.equals("0")) {
                    storage.setRecord(userid, "current_mode", null);
                    return INITIAL_INFO;
                }
                if(message.equals("checkout")){
                    String res = checkoutBucket(userid, storage);
                    storage.setRecord(userid, "current_mode", null);
                    if(res != null)
                        return res+"\n"+INITIAL_INFO;
                }
                try {
                    int number = Integer.parseInt(message);
                    List<GoodInStorage> goods = getGoods(storage);
                    boolean add = number > 0;
                    number = Math.abs(number);
                    if(number > goods.size())
                        return GOOD_NOT_EXIST+" "+number;
                    GoodInStorage gis = goods.get(number - 1);
                    if(add) {
                        if (gis.getCount() == 0)
                            return NO_GOOD_IN_STORE + gis.getName();
                        storage.setRecord(userid, "current_mode", "buy_actions_" + number);
                        return SELECT_COUNT_FOR_GOOD + gis.getName() + " или 0 чтобы вернуться к корзине.";
                    }else{
                        List<GoodInStorage> bucket = getBucket(userid, storage);
                        GoodInStorage fnd = null;
                        for(GoodInStorage gib: bucket)
                            if(gib.getName().equals(gis.getName())){
                                fnd = gib;
                                break;
                            }
                        if(fnd == null)
                            return BUCKET_NOT_FOUND+gis.getName();
                        storage.setRecord(userid, "current_mode", "bucket_remove_" + number);
                        return REMOVE_BUCKET_COUNT_FOR_GOOD + gis.getName() + " или 0 чтобы вернуться к корзине.";
                    }
                }catch(NumberFormatException nfe){
                    return NOT_A_NUMBER;
                }
            case "orders_actions":
                if(message.equals("0")) {
                    storage.setRecord(userid, "current_mode", null);
                    return INITIAL_INFO;
                }
                try {
                    int number = Integer.parseInt(message);
                    List<RecordInStorage> ords = getOrders(userid, storage);
                    if(number > ords.size() || number<1)
                        return ORDER_NOT_EXIST+" "+number;
                    RecordInStorage gis = ords.get(number-1);
                    StringBuilder sb = new StringBuilder();
                    gis.appendFullString(sb);
                    return sb+CHECK_MORE_ORDERS;
                }catch(NumberFormatException nfe){
                    return NOT_A_NUMBER;
                }
            case "deposit_actions":
                if(message.equals("0")) {
                    storage.setRecord(userid, "current_mode", null);
                    return INITIAL_INFO;
                }
                String used = storage.getRecord(userid, "keys");
                Set<String> userSet = used == null ? Collections.emptySet() : new HashSet<>(
                        Arrays.asList(
                                used.split("\\|")));
                message = message.trim();
                if(userSet.contains(message))
                    return "Код "+message+" был использован ранее. Введите другой или наберите 0 чтобы вернуться в меню.";
                String all = storage.getRecord(GENERIC_STORAGE, "keys");
                Set<String> allSet =  all == null ? Collections.emptySet() : new HashSet<>(
                        Arrays.asList(
                                all.split("\\|")));
                if(!allSet.contains(message))
                    return "Код "+message+" не валиден. Введите другой или наберите 0 чтобы вернуться в меню.";
                used = used == null ? message: used+"|"+message;
                storage.setRecord(userid, "keys", used);
                String wal = storage.getRecord(userid, "wallet");
                if(wal == null)
                    wal = "0";
                long mon = Long.parseLong(wal)+1000;
                setWallet(userid, storage, mon);
                return "Средства внесены, на счету "+mon+" у.е. Введите другой код или 0 чтобы вернуться в основное меню";
        }
        boolean buyActionsPrefix = mode.startsWith("buy_actions_");
        if(buyActionsPrefix || mode.startsWith("bucket_remove_")){
            List<GoodInStorage> goods = getGoods(storage);
            String numStr = mode.substring(buyActionsPrefix?12:14);
            int number = Integer.parseInt(numStr);
            StringBuilder sb = new StringBuilder();
            appendBuyMessage(userid, storage, sb);
            if(message.equals("0")) {
                storage.setRecord(userid, "current_mode", "buy_actions");
                return sb.toString();
            }
            message = message.trim();
            Integer count = 0;
            try{
                count = Integer.parseInt(message);
            }catch(NumberFormatException nfe){
                storage.setRecord(userid, "current_mode", "buy_actions");
                return NOT_A_NUMBER+"\n"+sb.toString();
            }
            if(count < 0) {
                storage.setRecord(userid, "current_mode", "buy_actions");
                return NUMBER_MUST_BE_POSITIVE+"\n"+sb.toString();
            }
            long wallet = getWallet(userid, storage)-getBucketCost(userid, storage);
            if(buyActionsPrefix) {
                GoodInStorage gis = goods.get(number - 1);
                if (gis.getCount() < count) {
                    storage.setRecord(userid, "current_mode", "buy_actions");
                    return NO_GOOD_IN_STORE + gis.getName() + "\n" + sb.toString();
                }
                if (wallet < count * gis.getPrice()) {
                    storage.setRecord(userid, "current_mode", "buy_actions");
                    return NO_MONEY + "\n" + sb.toString();
                }
                addToBucket(userid, storage, gis, count);
            }else{
                GoodInStorage gis = goods.get(number - 1);
                GoodInStorage st = null;
                List<GoodInStorage> bucket = getBucket(userid, storage);
                for(GoodInStorage gib: bucket) {
                    st = gib;
                    break;
                }
                if (st.getCount() < count) {
                    storage.setRecord(userid, "current_mode", "buy_actions");
                    return NO_GOOD_IN_BUCKET + gis.getName() + "\n" + sb.toString();
                }
                st.addCount(-1*count);
                if(st.getCount() == 0)
                    bucket.remove(st);
                setBucket(userid, storage, bucket);
            }
            storage.setRecord(userid, "current_mode", "buy_actions");

            sb.setLength(0);
            appendBuyMessage(userid, storage, sb);
            if(buyActionsPrefix)
                return GOODS_ADDED+"\n"+sb.toString();
            return GOODS_BUCKET_REMOVED+"\n"+sb.toString();
        }
        return null;
    }

    private final long getBucketCost(long userid, IStateStorage storage) {
        List<GoodInStorage> bucket = getBucket(userid, storage);
        long cost = 0;
        for(GoodInStorage gib: bucket)
            cost += gib.getCount()*gib.getPrice();
        return cost;
    }

    private final void setWallet(long userid, IStateStorage storage, long mon) {
        storage.setRecord(userid, "wallet", Long.toString(mon));
    }

    private final void addToBucket(long userid, IStateStorage storage, GoodInStorage tgis, int count) {
        List<GoodInStorage> bucket = getBucket(userid, storage);
        try {
            for (GoodInStorage gis : bucket)
                if (gis.getName().equals(tgis.getName())) {
                    gis.addCount(count);
                    return;
                }
            GoodInStorage gis = new GoodInStorage(tgis.getName(),
                    tgis.getDescription(),
                    count,
                    tgis.getPrice());
            if(bucket.isEmpty())
                bucket = new ArrayList<>();
            bucket.add(gis);
        }finally{
            setBucket(userid, storage, bucket);
        }
    }


    private final String checkoutBucket(long userid, IStateStorage storage) {
        List<GoodInStorage> goods = getGoods(storage);
        List<GoodInStorage> bucket = getBucket(userid, storage);
        long wallet = getWallet(userid, storage);
        if(bucket.isEmpty()){
            return EMPTY_BUCKET;
        }
        for(GoodInStorage gis: bucket){
            wallet -= gis.getCount()*gis.getPrice();
            for(GoodInStorage good: goods) {
                if (good.getName().equals(gis.getName())) {
                    good.addCount(gis.getCount() * -1);
                    if(good.getCount()<0)
                        return NO_GOOD_IN_STORE+good.getName();
                    break;
                }
            }
        }
        setWallet(userid, storage, wallet);
        setGoods(storage, goods);
        flushBucket(userid, storage);
        return addOrder(userid, storage, bucket);
    }

    private final void flushBucket(long userid, IStateStorage storage) {
        setBucket(userid, storage, new ArrayList<>());
    }

    private final String handleNullMode(String message, long userid, IStateStorage storage) {
        StringBuilder sb;
        switch (message.trim().toLowerCase()){
            case "buy":
                storage.setRecord(userid, "current_mode", "buy_actions");
                sb = new StringBuilder();
                appendBuyMessage(userid, storage, sb);
                return sb.toString();

            case "orders":
                storage.setRecord(userid, "current_mode", "orders_actions");
                sb = new StringBuilder("Имеющиесся заказы: \n");
                List<RecordInStorage> olist = getOrders(userid, storage);
                int i = 0;
                for(RecordInStorage ord: olist)
                {
                    i++;
                    sb.append(i);
                    sb.append(". ");
                    ord.appendShortString(sb);
                    sb.append("\n");
                }
                if(olist.isEmpty())
                    sb.append("Заказов нет, отправьте 0 чтобы вернуться в основное меню.");
                else
                    sb.append("Напишите номер интересующего вас заказа или напишите 0 чтобы вернуться обратно.");
                return sb.toString();

            case "deposit":
                storage.setRecord(userid, "current_mode", "deposit_actions");
                return "Чтобы пополнить счет нужно ввести код для пополнения или 0 чтобы вернуться в основное меню";

            case "account":
                sb = new StringBuilder();
                String money = storage.getRecord(userid, "wallet");
                sb.append("У вас на счету ");
                sb.append(money == null ? "0" : money);
                sb.append(" у.е.");
                return sb.toString()+" Доступные команды: \n"+COMMANDS;
        }
        return null;
    }

    private final void appendBuyMessage(long userid, IStateStorage storage, StringBuilder sb) {
        long mon = getWallet(userid, storage);
        sb.append("На счету ");
        sb.append(mon);
        sb.append(" у.е.\n");
        sb.append("Товары:\n");
        appendGoods(userid, storage,sb);
        sb.append(BUCKET_MESSAGE);
        appendBucketContent(userid, storage, sb);
    }

    private final long getWallet(long userid, IStateStorage storage) {
        String wal = storage.getRecord(userid, "wallet");
        if(wal == null)
            wal = "0";
        return Long.parseLong(wal);
    }

    private final void appendGoods(long userid, IStateStorage storage, StringBuilder sb){
        List<GoodInStorage> glist = getGoods(storage);
        int i = 0;
        for(GoodInStorage gis: glist){
            i++;
            sb.append(i);
            sb.append(". ");
            gis.appendOfferString(sb);
            sb.append("\n");
        }
        if(glist.isEmpty())
            sb.append("Товаров нет.\n");
    }
    private final void appendBucketContent(long userid, IStateStorage storage, StringBuilder sb) {
        List<GoodInStorage> ret = getBucket(userid, storage);
        sb.append("\nКорзина:");
        long cost = 0;
        if(ret.size() == 0)
            sb.append("\n Пусто");
        else
            for(GoodInStorage gis: ret) {
                cost += gis.getCount()*gis.getPrice();
                sb.append("\n");
                gis.appendOfferString(sb);
            }
        sb.append("\n Общая стоимость корзины: ");
        sb.append(cost);
    }
    private final List<GoodInStorage> getBucket(long userid, IStateStorage storage) {
        String goods = storage.getRecord(userid, "bucket");
        List<GoodInStorage> ret = JSON.parseArray(goods, GoodInStorage.class);
        if(ret == null)
            ret = Collections.emptyList();
        return ret;
    }
    private final void setBucket(long userid, IStateStorage storage, List<GoodInStorage> bucket) {
        storage.setRecord(userid, "bucket", JSON.toJSONString(bucket));
    }
    private final List<GoodInStorage> getGoods(IStateStorage storage) {
        String goods = storage.getRecord(GENERIC_STORAGE, "goods");
        List<GoodInStorage> ret = JSON.parseArray(goods, GoodInStorage.class);
        if(ret == null)
            ret = Collections.emptyList();
        return ret;
    }

    private final void setGoods(IStateStorage storage, List<GoodInStorage> goods){
        storage.setRecord(GENERIC_STORAGE, "goods", JSON.toJSONString(goods));
    }
    private final String addOrder(long userid, IStateStorage storage, List<GoodInStorage> bucket){
        String goods = storage.getRecord(userid, "orders");
        String maxids = storage.getRecord(GENERIC_STORAGE, "orders_max_id");
        int maxid = 0;
        try
        {
            maxid = Integer.parseInt(maxids);
        }catch (NumberFormatException nfe){
        }
        List<RecordInStorage> ret = JSON.parseArray(goods, RecordInStorage.class);
        if(ret == null)
            ret = new LinkedList<>();
        RecordInStorage newRec = RecordInStorage.getNewRecord(maxid);
        ret.add(newRec);
        storage.setRecord(GENERIC_STORAGE, "orders_max_id", String.valueOf(maxid+1));
        storage.setRecord(userid, "orders", JSON.toJSONString(ret));
        storage.setRecord(userid, "order_"+maxid, JSON.toJSONString(bucket));
        return "Заказ успешно добавлен, номер заказа: "+maxid;
    }

    private final List<RecordInStorage> getOrders(long userid, IStateStorage storage) {
        String goods = storage.getRecord(userid, "orders");
        List<RecordInStorage> gds = JSON.parseArray(goods, RecordInStorage.class);
        if(gds == null)
            return Collections.emptyList();
        for(RecordInStorage ris: gds) {
            String storedOrd = storage.getRecord(userid, "order_"+ris.getId());
            if(storedOrd == null)
                storedOrd = "[]";
            ris.setGoods(JSON.parseArray(storedOrd, GoodInStorage.class));
        }
        return gds;
    }
}