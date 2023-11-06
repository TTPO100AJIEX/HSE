package server;

import java.io.BufferedReader;
import java.io.DataOutput;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;

public class Client extends Thread
{
    private ClientsHolder clients;
    private String name = "Unknown";
    private BufferedReader in;
    private DataOutput out;
    public Client(Socket socket, ClientsHolder clients)
    {
        this.clients = clients;
        this.clients.add(this);
        try
        {
            this.in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            this.out = new DataOutputStream(socket.getOutputStream());
        } catch(IOException err) { }
    }
    
    @Override
    public void run()
    {
        try
        {
            this.name = this.in.readLine();
            this.clients.send(this.name + " joined");
            while (true)
            {
                String message = this.in.readLine();
                if (message == null) break;
                this.clients.send(this.name + ": " + message);
            }
        } catch(IOException err) { }
    }

    public void send(String message) throws IOException
    {
        this.out.writeBytes(message + "\n");
    }
}
