package reversi.Events.Render;

import reversi.Events.Event;

public final class Clear extends Event {
    public Clear() {
        this.eventType = Type.CLEAR;
    }

    @Override
    public String toString() {
        return "CLEAR";
    }
}
