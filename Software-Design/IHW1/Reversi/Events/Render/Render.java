package reversi.Events.Render;

import reversi.Events.Event;
import reversi.utils.Renderer.Renderer;

public final class Render extends Event
{
    private final Renderer.Context context;
    public Render(Renderer.Context context) { this.eventType = Type.RENDER; this.context = context; }

    public Renderer.Context getContext() { return this.context; }

    @Override public String toString() { return "RENDER"; }
}
