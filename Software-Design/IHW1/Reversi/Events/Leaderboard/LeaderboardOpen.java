package reversi.Events.Leaderboard;

import reversi.Events.Event;

public final class LeaderboardOpen extends Event
{
    public LeaderboardOpen() { this.eventType = Type.LEADERBOARD_OPEN; }

    @Override public String toString() { return "LEADERBOARD_OPEN"; }
}
