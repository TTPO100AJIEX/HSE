package server;

import java.io.IOException;
import java.util.ArrayList;

public class ClientsHolder
{
    private ArrayList<Client> clients = new ArrayList<Client>();
    public synchronized void add(Client client) { this.clients.add(client); }
    private synchronized void remove(int index) { this.clients.remove(index); }

    public synchronized void send(String message)
    {
        for (int i = 0; i < this.clients.size(); i++)
        {
            Client client = this.clients.get(i);
            try { client.send(message); } catch(IOException err) { this.remove(i); i--; continue; }
        }
    }
}
