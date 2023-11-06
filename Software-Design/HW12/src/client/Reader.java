package client;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;

public class Reader extends Thread
{
    BufferedReader stream;
    String name;
    Reader(Socket socket, String name) throws IOException
    {
        this.name = name;
        this.stream = new BufferedReader(new InputStreamReader(socket.getInputStream()));
    }

    @Override
    public void run()
    {
        while (true)
        {
            try
            {
                String message = this.stream.readLine();
                if (message == null) break;
                System.out.println(message);
            } catch(IOException err) { }
        }
    }
}
