package reversi.Layers;

import reversi.Events.Event;

import java.util.function.Consumer;

public class Layer {
    protected final Consumer<Event> eventDispatcher;

    public Layer(final Consumer<Event> eventDispatcher) {
        this.eventDispatcher = eventDispatcher;
    }
}