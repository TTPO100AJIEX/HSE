package observer;

import java.util.ArrayList;
import java.util.function.Consumer;

public class Console
{
    ArrayList<Consumer<Game>> subscribers = new ArrayList<Consumer<Game>>();
    public void subscribe(Consumer<Game> callback) { this.subscribers.add(callback); }

    public void newGame(Game game)
    {
        for (Consumer<Game> callback : this.subscribers) callback.accept(game);
    }
}
