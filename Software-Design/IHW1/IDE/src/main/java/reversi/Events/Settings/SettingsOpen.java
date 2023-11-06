package reversi.Events.Settings;

import reversi.Events.Event;

public final class SettingsOpen extends Event {
    public SettingsOpen() {
        this.eventType = Type.SETTINGS_OPEN;
    }

    @Override
    public String toString() {
        return "SETTINGS_OPEN";
    }
}
