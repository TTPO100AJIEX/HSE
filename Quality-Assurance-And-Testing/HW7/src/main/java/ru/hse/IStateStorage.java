package ru.hse;

public interface IStateStorage {
    public String getRecord(long userid, String key);
    public void setRecord(long userid, String key, String value);
}
