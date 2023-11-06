package folder;

import java.nio.file.Path;
import java.nio.file.Files;
import java.io.File;
import java.util.Scanner;
import java.io.IOException;
import java.util.Collection;
import java.util.HashSet;

public final class TextFile
{
    private final Path path;
    private HashSet<String> dependencies = new HashSet<String>();

    public TextFile(Path path)
    {
        this.path = path;
        try (Scanner reader = new Scanner(new File(path.toString())))
        {
            while (reader.hasNextLine())
            {
                String line = reader.nextLine();
                if (!line.startsWith("require '") || !line.endsWith("'")) continue;
                dependencies.add(line.substring(line.indexOf("'") + 1, line.lastIndexOf("'")));
            }
        } catch(Exception e) { }
    }

    public Collection<String> getDependencies() { return this.dependencies; }
    public String getContent()
    {
        try { return Files.readString(this.path); }
        catch(Exception e) { return ""; }
    }

    @Override public boolean equals(Object other) { return(other instanceof TextFile && ((TextFile)(other)).path.equals(this.path)); }
    @Override public int hashCode() { return this.path.hashCode(); }
    @Override public String toString() { return this.path.toString(); }
}
