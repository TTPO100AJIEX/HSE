package reversi.utils.Input;

public interface Input {
    int getInteger(final String query);

    String getString(final String query);

    char getChar(final String query);

    void getInput(final String query);
}