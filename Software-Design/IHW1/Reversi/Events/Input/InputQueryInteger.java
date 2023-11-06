package reversi.Events.Input;

import reversi.Events.Event;

public final class InputQueryInteger extends Event
{
    private final String query;
    private int value;
    public InputQueryInteger()             { this.eventType = Type.INPUT_QUERY_INTEGER; this.query = ""; }
    public InputQueryInteger(String query) { this.eventType = Type.INPUT_QUERY_INTEGER; this.query = query; }

    public String getQuery() { return this.query; }
    public int    getValue() { return this.value; }

    public void setValue(final int value) { this.value = value; }

    @Override public String toString() { return "INPUT_QUERY_INTEGER | " + this.query + " | " + this.value; }
}
