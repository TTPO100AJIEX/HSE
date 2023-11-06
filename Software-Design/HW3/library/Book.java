package library;

import java.util.ArrayList;

public final class Book
{
    private String name;
    private String description;
    private ArrayList<String> authors;
    private Integer year;

    public Book(String name, String description, ArrayList<String> authors, Integer year)
    {
        this.name = name;
        this.description = description;
        this.authors = authors;
        this.year = year;
    }
    public Book(Book other)
    {
        this.name = other.name;
        this.description = other.description;
        this.authors = other.authors;
        this.year = other.year;
    }

    public String getName() { return this.name; }
    public String getDescription() { return this.description; }
    public ArrayList<String> getAuthors() { return this.authors; }
    public Integer getYear() { return this.year; }

    @Override public boolean equals(Object other)
    {
        if (!(other instanceof Book)) return false;
        return (this.name == ((Book)(other)).name);
    }
    @Override public String toString()
    {
        String result = this.name + " " + this.description;
        for (String author : this.authors) result += " " + author;
        return result + " " + this.year.toString();
    }
}
