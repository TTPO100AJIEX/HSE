package reversi.Events.Window;

import reversi.Events.Event;

public final class WindowClose extends Event {
    public WindowClose() {
        this.eventType = Type.WINDOW_CLOSE;
    }

    @Override
    public String toString() {
        return "WINDOW_CLOSE";
    }
}
