package reversi.Events.Input;

import reversi.Events.Event;

public final class InputQueryChar extends Event
{
    private final String query;
    private char value;
    public InputQueryChar()             { this.eventType = Type.INPUT_QUERY_CHAR; this.query = ""; }
    public InputQueryChar(String query) { this.eventType = Type.INPUT_QUERY_CHAR; this.query = query; }

    public String getQuery() { return this.query; }
    public char   getValue() { return this.value; }

    public void setValue(final char value) { this.value = value; }

    @Override public String toString() { return "INPUT_QUERY_CHAR | " + this.query + " | " + this.value; }
}
