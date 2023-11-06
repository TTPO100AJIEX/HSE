package reversi.Events.Settings;

import reversi.Events.Event;

public final class SettingsClose extends Event
{
    public SettingsClose() { this.eventType = Type.SETTINGS_CLOSE; }

    @Override public String toString() { return "SETTINGS_CLOSE"; }
}
