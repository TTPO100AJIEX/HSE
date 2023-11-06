package server;

import java.io.IOException;
import java.net.ServerSocket;

public class Server
{
    Server() { }

    public void run(String address, int port) throws IOException
    {
        ServerSocket server = new ServerSocket(80);
        ClientsHolder clients = new ClientsHolder();
        while (true)
        {
            new Client(server.accept(), clients).start();
        }
    }
}
