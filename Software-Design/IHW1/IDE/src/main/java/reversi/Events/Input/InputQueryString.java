package reversi.Events.Input;

import reversi.Events.Event;

public final class InputQueryString extends Event {
    private final String query;
    private String value;

    public InputQueryString() {
        this.eventType = Type.INPUT_QUERY_STRING;
        this.query = "";
    }

    public InputQueryString(String query) {
        this.eventType = Type.INPUT_QUERY_STRING;
        this.query = query;
    }

    public String getQuery() {
        return this.query;
    }

    public String getValue() {
        return this.value;
    }

    public void setValue(final String value) {
        this.value = value;
    }

    @Override
    public String toString() {
        return "INPUT_QUERY_STRING | " + this.query + " | " + this.value;
    }
}
