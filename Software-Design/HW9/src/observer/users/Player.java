package observer.users;

import observer.Game;

public class Player extends User
{
    public Player(String name) { super(name); }

    @Override
    void output(Game game) { System.out.println("Player " + this.name + ": " + game.title + ", " + game.achievements); }
}
