package reversi.utils.Input;

public interface Input
{
    public int    getInteger(final String query);
    public String getString (final String query);
    public char   getChar   (final String query);
    public void   getInput  (final String query);
}