package reversi.Layers;

import java.util.function.Consumer;

import reversi.Events.Event;

public class Layer
{
    protected final Consumer<Event> eventDispatcher;
    public Layer(final Consumer<Event> eventDispatcher) { this.eventDispatcher = eventDispatcher; }
}