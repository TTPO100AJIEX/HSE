package observer.users;

import observer.Game;

public class Developer extends User
{
    public Developer(String name) { super(name); }

    @Override
    void output(Game game) { System.out.println("Developer " + this.name  + ": " + game.data); }
}
