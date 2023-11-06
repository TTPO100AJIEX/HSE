package reversi;

import java.util.function.Consumer;

import reversi.Events.Event;
import reversi.Events.Window.WindowClose;
import reversi.Events.Window.WindowOpen;
import reversi.utils.Renderer.Renderer;
import reversi.utils.Renderer.ConsoleRenderer;
import reversi.utils.Input.Input;
import reversi.utils.Input.ConsoleInput;

final class Window
{
    final Consumer<Event> eventDispatcher;
    public Window(final Consumer<Event> eventDispatcher) { this.eventDispatcher = eventDispatcher; }

    public void open()  { this.eventDispatcher.accept(new WindowOpen()); }
    public void close() { this.eventDispatcher.accept(new WindowClose()); }

    private final Renderer renderer = new ConsoleRenderer();
    public void clear() { renderer.clear(); }
    public void render(final Renderer.Context context) { renderer.render(context); }

    private final Input input = new ConsoleInput();
    public int    getInteger(final String query)  { return input.getInteger(query); }
    public String getString (final String query)  { return input.getString (query); }
    public char   getChar   (final String query)  { return input.getChar   (query); }
    public void   getInput  (final String query)  { input.getInput(query); }
}
