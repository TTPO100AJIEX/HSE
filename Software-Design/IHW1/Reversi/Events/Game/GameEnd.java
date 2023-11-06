package reversi.Events.Game;

import reversi.Events.Event;
import reversi.Layers.Leaderboard;

public class GameEnd extends Event
{
    private Leaderboard.Record record;
    public GameEnd(Leaderboard.Record record) { this.eventType = Type.GAME_END; this.record = record; }

    public Leaderboard.Record getRecord() { return this.record; }

    @Override public String toString() { return "GAME_END"; }
}
