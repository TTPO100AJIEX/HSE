package LMS;

import java.util.Scanner;

public class LMS
{
    public static void main(String[] args)
    {
        Group group;
        try { group = new Group("BPI223"); } catch(Exception e) { System.out.println(e); return; }

        Scanner commands = new Scanner(System.in);
        command_reader: while (true)
        {
            String command = commands.nextLine();
            switch (command)
            {
                case "/q": { break command_reader; }
                case "/r":
                {
                    Student student = group.getRandom();
                    System.out.println("Отвечает " + student);
                    System.out.println("Присутствует ли на паре (y/n)?");
                    String state = commands.nextLine();
                    if (state.equals("n")) { student.setAbsent(); break; }
                    student.setPresent();
                    System.out.println("Оценка за ответ: ");
                    student.addMark(commands.nextInt());
                    break;
                }
                case "/l": { System.out.println(group); break; }
                default:
                {
                    System.out.println();
                    System.out.println("1. /r - choose random student");
                    System.out.println("2. /l - list of student with grades");
                    System.out.println("3. /q - stop");
                    break;
                }
            }
        }
        commands.close();

        try { group.output(); } catch(Exception e) { System.out.println(e); return; }
    }
}
