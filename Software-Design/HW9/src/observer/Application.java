package observer;

import observer.users.User;
import observer.users.Player;
import observer.users.Journalist;
import observer.users.Developer;

public class Application
{
    public static void main(String[] args)
    {
        Console console = new Console();

        User[] users =
        {
            new Player("1"), new Player("2"), new Player("3"),
            new Journalist("1"), new Journalist("2"), new Journalist("3"),
            new Developer("1"), new Developer("2"), new Developer("3")
        };

        for (User user : users) user.observe(console);

        console.newGame(new Game("Title 1", "Achievements 1", "Description 1", "Data 1"));
        console.newGame(new Game("Title 2", "Achievements 2", "Description 2", "Data 2"));
    }
}