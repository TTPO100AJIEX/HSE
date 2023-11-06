package reversi.Events.Input;

import reversi.Events.Event;

public final class InputQuery extends Event {
    private final String query;

    public InputQuery() {
        this.eventType = Type.INPUT_QUERY;
        this.query = "";
    }

    public InputQuery(String query) {
        this.eventType = Type.INPUT_QUERY;
        this.query = query;
    }

    public String getQuery() {
        return this.query;
    }

    @Override
    public String toString() {
        return "INPUT_QUERY | " + this.query;
    }
}
