package LMS;

import java.util.ArrayList;

public class Student
{
    private String name;
    private Byte present = 2;
    private ArrayList<Integer> marks = new ArrayList<Integer>();
    
    public Student(String name)
    {
        this.name = name;
    }

    public Byte Presence() { return this.present; }
    public void setAbsent() { this.present = 0; }
    public void setPresent() { this.present = 1; }

    public void addMark(Integer mark) { this.marks.add(mark); }
    
    @Override
    public String toString()
    {
        String marks = this.marks.toString();
        return this.name + ": " + marks.substring(1, marks.length() - 1);
    }
}
