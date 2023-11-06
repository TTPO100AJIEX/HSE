package multithreading;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class Test {
    
    private static List<String> data = Arrays.asList(
            "aa", "bbb", "c", "d", "aa", "bbb", "c", "d", "aa", "bbb", "c", "d", "aa", "bbb", "c", "d",
            "aa", "bbb", "c", "d", "aa", "bbb", "c", "d", "aa", "bbb", "c", "d", "aa", "bbb", "c", "d",
            "aa", "bbb", "c", "d", "aa", "bbb", "c", "d", "aa", "bbb", "c", "d", "aa", "bbb", "c", "d",
            "aa", "bbb", "c", "d", "aa", "bbb", "c", "d", "aa", "bbb", "c", "d", "aa", "bbb", "c", "d");

    public static void main(String[] args) throws InterruptedException
    {
        System.out.println(new Solver(Test.data, 3).getAnswer());

        List<String> test = new ArrayList<>();
        for (int i = 0; i < 1e8; i++) test.add("abc");
        System.out.println(new Solver(test, 3).getAnswer());
    }
}