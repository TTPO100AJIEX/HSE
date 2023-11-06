package server;

import java.io.IOException;

public class Application
{
    public static void main(String[] args) throws IOException
    {
        new Server().run("localhost", 80);
    }
}
