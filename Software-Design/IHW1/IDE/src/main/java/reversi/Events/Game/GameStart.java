package reversi.Events.Game;

import reversi.Events.Event;

public class GameStart extends Event {
    public GameStart() {
        this.eventType = Type.GAME_START;
    }

    @Override
    public String toString() {
        return "GAME_START";
    }
}
