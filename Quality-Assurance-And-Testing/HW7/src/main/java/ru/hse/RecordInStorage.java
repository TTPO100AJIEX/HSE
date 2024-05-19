package ru.hse;

import com.alibaba.fastjson2.annotation.JSONField;

import java.util.Iterator;
import java.util.List;

public class RecordInStorage {
    @JSONField(name = "id", required = true)
    private int id = -1;
    private List<GoodInStorage> goods;
    private RecordInStorage(int id)
    {
        this.id = id;
    }
    public  List<GoodInStorage> getGoods(){
        return goods;
    }
    public void setGoods(List<GoodInStorage> goods){
        this.goods = goods;
    }
    public static RecordInStorage getNewRecord(int id){
        return new RecordInStorage(id);
    }

    public void appendShortString(StringBuilder sb) {
        Iterator<GoodInStorage> git = goods.iterator();
        for(int i = 0;i<100;i++){
            if(!git.hasNext())
                return;
            if(i>0)
                sb.append(", ");
            GoodInStorage gis = git.next();
            String name = gis.getName();
            if(i+name.length()>20)
                name = name.substring(0, 20-i)+"..";
            sb.append(name);
            sb.append("(");
            sb.append(gis.getCount());
            sb.append(")");
        }
    }

    public void appendFullString(StringBuilder sb) {
        boolean first = true;
        for(GoodInStorage gis: goods){
            if(!first)
                sb.append(",\n");
            else
                first = false;
            sb.append(gis.getName());
            sb.append("(");
            sb.append(gis.getCount());
            sb.append(") - ");
            String desc = gis.getDescription();
            if(desc == null)
                desc = "Описания нет";
            sb.append(desc);
        }
        sb.append("\n");
    }

    public int getId() {
        return id;
    }
}
