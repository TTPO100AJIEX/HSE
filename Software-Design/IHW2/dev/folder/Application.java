package folder;

import java.nio.file.Paths;
import java.util.Optional;

public final class Application
{
    public static void main(String[] args)
    {
        Folder folder = new Folder(Paths.get("root")); // Read the folder and build the list of files with dependencies
        Optional<FolderError> error = folder.check(); // Check if anything is wrong
        if (error.isPresent()) { System.out.println(error.get()); return; } // If somethig is wrong, print the error and stop
        System.out.println(folder.concatenate()); // Print the answer
    }

    private Application() { }
}