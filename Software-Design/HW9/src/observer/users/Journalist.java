package observer.users;

import observer.Game;

public class Journalist extends User
{
    public Journalist(String name) { super(name); }

    @Override
    void output(Game game) { System.out.println("Journalist " + this.name + ": " + game.description); }
}
