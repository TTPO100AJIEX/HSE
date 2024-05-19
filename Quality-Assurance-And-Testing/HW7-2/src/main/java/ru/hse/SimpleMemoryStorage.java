package ru.hse;

import java.util.HashMap;
import java.util.Map;

public class SimpleMemoryStorage implements IStateStorage{
    private static long GENERIC_STORAGE = 0l;
    private Map<Long, Map<String, String>> data = new HashMap<>();
    public SimpleMemoryStorage(){
        this(false);
    }
    public SimpleMemoryStorage(boolean empty){
        //initial test data
        Map<String, String> initialData = new HashMap<>();
        data.put(0L, initialData);
        if(!empty) {
            initialData.put("keys", "3DRwBBrcFThKXq9zNIdPihfg3eaQ7g|0awdnDr9kaByy4qg4ha94A2HXcRCJz|lyvQuGNBYhrcnDfCsn9cchqrTvLEtg|qdrcNtvcZIZB9H3ylKPoG4pM1FFQgB|b4zK3bHKAnUMaHCuKpfKHBMzPCAxBs|hWxisKcexAgUtvWKs4NDdge39iSAsm|G8BasIDVT86HaJNWCDBzoZ4sF1sYZA|1vVPsiFdUPOIAMIzsnMrDppdGff9cY|pNs8UA725BryPtCSmbNQtelaQfsh90|HPtxlAwkBjiE8r6gd7Ba9SnFU3cosF");
            initialData.put("goods", "[" +
                "{" +
                " 'name':'Good with high price'," +
                " 'description':'some strange thing'," +
                " 'count':100," +
                " 'price':15" +
                "}," +
                "{" +
                " 'name':'good2'," +
                " 'description':'some more strange things'," +
                " 'count':200," +
                " 'price':5" +
                "}," +
                "{" +
                " 'name':'good3 with long and strange description for no purpose and mean'," +
                " 'description':'some more strange things for more strange reasons'," +
                " 'count':2000," +
                " 'price':1" +
                "}" +
                "]");
        }
    }

    @Override
    public String getRecord(long userid, String key) {
        if(key == null)
            return null;
        Map<String, String> udata = data.get(userid);
        if(udata == null)
            return null;
        return udata.get(key);
    }

    @Override
    public void setRecord(long userid, String key, String value) {
        Map<String, String> udata =data.get(userid);
        if(udata == null) {
            udata = new HashMap<>();
            data.put(userid, udata);
        }
        udata.put(key, value);
    }
}
