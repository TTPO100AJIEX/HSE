package library;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Hashtable;
import java.util.Map;
import java.util.Scanner;
import java.util.Set;

public final class Library implements Map< String, ArrayList<Book> >
{
    private Hashtable< String, ArrayList<Book> > data = new Hashtable< String, ArrayList<Book> >();

    @Override public int size() { return this.books().size(); }
    @Override public boolean isEmpty() { return this.size() == 0; }

    @Override public boolean containsKey(Object key) { return (this.data.containsKey(key) && this.data.get(key).size() != 0); }
    public boolean has(Book book)
    {
        ArrayList<Book> books = this.get(book.getName());
        for (Book stored : books)
        {
            if (book.equals(stored)) return true;
        }
        return false;
    }
    @Override public boolean containsValue(Object value) { return (value instanceof Book && this.has((Book)(value))); }

    public Book getBook(Book book)
    {
        ArrayList<Book> books = this.get(book.getName());
        for (Book stored : books)
        {
            if (book.equals(stored)) return stored;
        }
        return null;
    }
    @Override public ArrayList<Book> get(Object key)
    {
        if (!this.containsKey(key)) this.put((String)(key), new ArrayList<Book>());
        return this.data.get(key); }
    @Override public Set<String> keySet() { return this.data.keySet(); }
    @Override public Collection< ArrayList<Book> > values() { return this.data.values(); }
    public Collection<Book> books()
    {
        ArrayList<Book> answer = new ArrayList<Book>();
        for (ArrayList<Book> bucket : this.values())
        {
            for (Book book : bucket) answer.add(book);
        }
        return answer;
    }
    
    @Override 
    public Set< Entry< String, ArrayList<Book> > > entrySet() { return this.data.entrySet(); }

    public void addBook(String name, String description, ArrayList<String> authors, Integer year) { this.addBook(new Book(name, description, authors, year)); }
    public void addBook(Book book)
    {
        if (!this.containsKey(book.getName())) this.put(book.getName(), new ArrayList<Book>());
        this.get(book.getName()).add(book);
    }
    @Override public ArrayList<Book> put(String key, ArrayList<Book> value) { return this.data.put(key, value); }
    @Override public void putAll(Map<? extends String, ? extends ArrayList<Book>> m) { this.data.putAll(m); }

    public Book removeBook(Book book)
    {
        ArrayList<Book> books = this.get(book.getName());
        for (Book stored : books)
        {
            if (book.equals(stored)) { books.remove(stored); return stored; }
        }
        return null;
    }
    @Override public ArrayList<Book> remove(Object key) { return this.data.remove(key); }
    @Override public void clear() { this.data.clear(); }

    public void fill() throws IOException
    {
        File file = new File("data/books.txt");
        if (!file.exists()) throw new IOException("Could not find the list of books!");
        if (!file.canRead()) throw new IOException("Unable to read the list of books!");
        Scanner reader = new Scanner(file);
        while (reader.hasNextLine())
        {
            String name = reader.next(), description = reader.next();
            ArrayList<String> authors = new ArrayList<String>();
            int amount = reader.nextInt();
            for (int i = 0; i < amount; i++) authors.add(reader.next());
            this.addBook(name, description, authors, reader.nextInt());
        }
        reader.close();
    }

    public void printAll()
    {
        if (this.size() == 0) System.out.println("No records have been found!");
        else for (Book book : this.books()) System.out.println(book);
    }
    public ArrayList<Book> getOptions(String name) { return this.get(name); }
    public Book takeBook(Book book) { return this.removeBook(book); }
}
