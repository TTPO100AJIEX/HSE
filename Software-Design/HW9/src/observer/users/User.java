package observer.users;

import observer.Console;
import observer.Game;

public abstract class User
{
    String name;
    public User(String name) { this.name = name; }

    abstract void output(Game game);
    public void observe(Console console) { console.subscribe(this::output); }
}
