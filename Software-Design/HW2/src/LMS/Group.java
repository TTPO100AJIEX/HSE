package LMS;

import java.io.IOException;
import java.io.File;
import static java.nio.charset.StandardCharsets.UTF_8;

import java.util.Scanner;
import java.io.FileWriter;

import java.util.ArrayList;
import java.util.stream.Collectors;

import java.util.Random;

public class Group
{
    private String name;
    ArrayList<Student> students = new ArrayList<Student>();

    public Group(String name) throws IOException
    {
        this.name = name;
        String filename = "data/src/" + name + ".txt";
        File file = new File(filename);
        if (!file.exists()) throw new IOException("Could not find the file  " + filename);
        if (!file.canRead()) throw new IOException("Unable to read the list of students from " + filename);

        Scanner reader = new Scanner(file, UTF_8);
        while (reader.hasNextLine()) this.students.add(new Student(reader.nextLine()));
        reader.close();
    }

    public Student getRandom() throws IllegalStateException
    {
        ArrayList<Integer> present_indexes = new ArrayList<Integer>();
        for (Integer i = 0; i < this.students.size(); i++)
        {
            if (this.students.get(i).Presence() != 0) present_indexes.add(i);
        }
        if (present_indexes.size() == 0) throw new IllegalStateException("There are not students in class");
        Random rand = new Random();
        return this.students.get(rand.nextInt(present_indexes.size()));
    }

    @Override
    public String toString()
    {
        return this.students.stream().map(Object::toString).collect(Collectors.joining("\n"));
    }
    public void output() throws IOException
    {
        String filename = "data/res/" + name + ".txt";
        FileWriter writer = new FileWriter(filename, UTF_8);
        writer.write(this.toString());
        writer.close();
    }
}
