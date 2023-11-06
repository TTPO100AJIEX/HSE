package reversi.Events.Window;

import reversi.Events.Event;

public final class WindowOpen extends Event
{
    public WindowOpen() { this.eventType = Type.WINDOW_OPEN; }

    @Override public String toString() { return "WINDOW_OPEN"; }
}
