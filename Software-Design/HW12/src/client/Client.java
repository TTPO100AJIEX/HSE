package client;

import java.io.IOException;
import java.net.Socket;

public class Client
{
    String name;
    Client(String name) { this.name = name; }

    Socket socket;
    Reader reader;
    Writer writer;
    public void run(String address, int port) throws IOException, InterruptedException
    {
        this.socket = new Socket(address, port);
        this.reader = new Reader(this.socket, this.name);
        this.writer = new Writer(this.socket, this.name);
        this.reader.start(); this.writer.start();
        this.reader.join(); this.writer.join();
    }
}
