package reversi.Events.Control;

import reversi.Events.Event;

public final class SwitchControl extends Event {
    private final Parts part;

    public SwitchControl(Parts part) {
        this.eventType = Type.SWITCH_CONTROL;
        this.part = part;
    }

    public Parts getPart() {
        return this.part;
    }

    @Override
    public String toString() {
        return "SWITCH_CONTROL to " + this.part.toString();
    }

    public enum Parts {SETTINGS, LEADERBOARD, PLAY_EASY, PLAY_ADVANCED, PLAY_PLAYER, PLAY_RANDOM, QUIT}
}
