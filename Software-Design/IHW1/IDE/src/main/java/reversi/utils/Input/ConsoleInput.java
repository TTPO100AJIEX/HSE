package reversi.utils.Input;

import java.util.Scanner;

public final class ConsoleInput implements Input {
    private final Scanner scanner = new Scanner(System.in);

    @Override
    public int getInteger(final String query) {
        return this.getInteger(query, false);
    }

    @Override
    public String getString(final String query) {
        System.out.print(query);
        try {
            return this.scanner.next();
        } catch (Exception e) {
            return "";
        }
    }

    @Override
    public char getChar(final String query) {
        System.out.print(query);
        try {
            return this.scanner.next().charAt(0);
        } catch (Exception e) {
            return '\0';
        }
    }

    @Override
    public void getInput(final String query) {
        System.out.print(query);
        try {
            System.in.read();
        } catch (Exception e) {
        }
    }

    private int getInteger(final String query, boolean is_repeated) {
        System.out.print(query);
        try {
            return this.scanner.nextInt();
        } catch (Exception err) {
            try {
                this.scanner.next();
            } catch (Exception e) {
            }
            if (is_repeated) return this.getInteger(query, true);
            else
                return this.getInteger("\033[0;31mThe input is wrong! Please, enter an integer!\033[0m\n " + query, true);
        }
    }
}
