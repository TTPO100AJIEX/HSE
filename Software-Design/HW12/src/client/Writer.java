package client;

import java.io.DataOutput;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.util.Scanner;

public class Writer extends Thread
{
    DataOutput stream;
    String name;
    Writer(Socket socket, String name) throws IOException
    {
        this.name = name;
        this.stream = new DataOutputStream(socket.getOutputStream());
    }

    @Override
    public void run()
    {
        try { this.stream.writeBytes(this.name + "\n"); } catch(IOException err) { }
        Scanner scanner = new Scanner(System.in);
        while (true)
        {
            try { this.stream.writeBytes(scanner.nextLine() + "\n"); } catch(IOException err) { }
        }
    }
}
