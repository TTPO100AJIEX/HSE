package reversi.Events.Leaderboard;

import reversi.Events.Event;

public final class LeaderboardClose extends Event {
    public LeaderboardClose() {
        this.eventType = Type.LEADERBOARD_CLOSE;
    }

    @Override
    public String toString() {
        return "LEADERBOARD_CLOSE";
    }
}
