package library;

import java.io.IOException;

public final class Application
{
    public static void main(String[] args) throws IOException
    {
        Library library = new Library();
        try { library.fill(); } catch(Exception e)
        {
            System.out.println("Failed to read the books!");
            System.out.println(e);
            return;
        }
        new Reader().run(library);
    }

    private Application() { }
}