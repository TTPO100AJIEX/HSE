package folder;

public class FolderError
{
    public String description;
    
    public FolderError(String description) { this.description = description; }

    @Override public String toString() { return this.description; }
}
