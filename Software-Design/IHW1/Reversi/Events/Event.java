package reversi.Events;

public abstract class Event
{
    public static enum Type
    {
        WINDOW_OPEN, WINDOW_CLOSE,
        SETTINGS_OPEN, SETTINGS_CLOSE,
        LEADERBOARD_OPEN, LEADERBOARD_CLOSE,
        GAME_START, GAME_END,
        SWITCH_CONTROL,

        INPUT_QUERY_INTEGER, INPUT_QUERY_STRING, INPUT_QUERY_CHAR, INPUT_QUERY,
        RENDER, CLEAR
    };

    protected Type eventType;
    public final Type getEventType() { return(this.eventType); }

    @Override public abstract String toString();
}
