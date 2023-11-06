package client;

import java.io.IOException;

public class Application
{
    public static void main(String[] args) throws IOException, InterruptedException
    {
        new Client(args[0]).run("localhost", 80);
    }
}
