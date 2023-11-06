package library;

import java.util.ArrayList;
import java.util.Scanner;

public class Reader
{
    ArrayList<Book> books = new ArrayList<Book>();
    public Reader() { }
    
    public void run(Library library)
    {
        Scanner commands = new Scanner(System.in);
        command_reader: while (true)
        {
            String command = commands.next();
            switch (command)
            {
                case "quit": { break command_reader; }
                case "get":
                {
                    ArrayList<Book> books = library.getOptions(commands.next());
                    if (books.size() == 0)System.out.println("No records have been found!");
                    else
                    {
                        for (int i = 0; i < books.size(); i++) System.out.println(Integer.toString(i + 1) + ". " + books.get(i));
                        try
                        {
                            int value = commands.nextInt() - 1;
                            this.books.add(library.takeBook(books.get(value)));
                        } catch(Exception e) { System.out.println("Something went wrong! Please, try again!"); }
                    }
                    break;
                }
                case "put":
                {
                    try
                    {
                        int value = commands.nextInt() - 1;
                        library.addBook(this.books.get(value));
                        this.books.remove(value);
                    } catch(Exception e) { System.out.println("Something went wrong! Please, try again!"); }
                    break;
                }
                case "list":
                {
                    if (this.books.size() == 0) System.out.println("No records have been found!");
                    else for (Book book : this.books) System.out.println(book);
                    break;
                }
                case "all": { library.printAll(); break; }
                default: { System.out.println("Invalid command"); }
            }
        }
        commands.close();
    }
}
