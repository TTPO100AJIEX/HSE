package ru.hse;

import com.alibaba.fastjson2.annotation.JSONField;

public class GoodInStorage {
    @JSONField(name = "name")
    private String name;
    @JSONField(name = "description")
    private String description;
    @JSONField(name = "count")
    private int count;
    @JSONField(name = "price")
    private int price;
    public  GoodInStorage(String name, String desc, int count, int price){
        this.name = name;
        this.description = desc;
        this.count = count;
        this.price = price;
    }
    public int getCount(){
        return count;
    }
    public int getPrice(){
        return price;
    }
    public String getName(){
        return name;
    }
    public String getDescription(){
        return description;
    }
    public void appendOfferString(StringBuilder sb) {
        sb.append(name);
        sb.append("(");
        sb.append(count);
        sb.append(" ед.) за ");
        sb.append(price);
        sb.append("у.е.");
    }

    public void addCount(int count) {
        this.count += count;
    }
}
