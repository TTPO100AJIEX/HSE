/*
 * Folder
 * v1.0.0
 * Copyright 2022 TTPO100AJIEX
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR 
 * IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
package folder;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.Map.Entry;
import java.util.Optional;

/*
 * 
 */
public final class Folder
{
    private final Path path;
    private HashMap<TextFile, Integer> files = new HashMap<TextFile, Integer>();
    public Folder(Path path)
    {
        this.path = path;
        this.searchFiles(this.path);
    }
    private void searchFiles(Path dirpath)
    {
        try {
            for (Path filename : Files.newDirectoryStream(dirpath))
            {
                if (Files.isDirectory(filename)) this.searchFiles(filename);
                else this.files.put(new TextFile(filename), -1);
            }
        } catch(Exception e) { }
    }
    
    private int calculteOrderingNextIndex = -1;
    private void calculteOrdering(Entry<TextFile, Integer> now)
    {
        if (now.getValue() < -2) return; // This vertex has been marked as invalid before
        if (now.getValue() == -2) { now.setValue(-3); return; } // This vertex is entered => loop detected
        if (now.getValue() != -1) return; // Has been visited and exited
        now.setValue(-2); // entered
        dependencies: for (String file : now.getKey().getDependencies())
        {
            Path path = Paths.get(this.path.toString() + "/" + file);
            for (Entry<TextFile, Integer> stored : this.files.entrySet())
            {
                if (stored.getKey().equals(new TextFile(path)))
                {
                    int storedValue = stored.getValue();
                    this.calculteOrdering(stored);
                    if (storedValue != -3 && stored.getValue() == -3) now.setValue(-3);
                    continue dependencies;
                }
            }
            now.setValue(-4); // Invalid dependency detected
            return;
        }
        if (now.getValue() != -2) return;
        now.setValue(this.calculteOrderingNextIndex--);
    }
    private void calculteOrdering()
    {
        if (this.files.isEmpty() || this.calculteOrderingNextIndex != -1) return;
        this.calculteOrderingNextIndex = this.files.size();
        for (Entry<TextFile, Integer> file : this.files.entrySet()) this.calculteOrdering(file);
    }

    public Optional<FolderError> check()
    {
        this.calculteOrdering();
        String error = "";
        for (Entry<TextFile, Integer> file : this.files.entrySet())
        {
            switch(file.getValue())
            {
                case -4: { error += "File " + file.getKey() + " has an invalid dependency!\n"; break; }
                case -3: { error += "File " + file.getKey() + " paricipates in a circular dependency!\n"; break; }
                case -2: case -1: case 0: { error += "An unknown error has occurred! Please try again!\n"; break; }
                default: { }
            }
        }
        return Optional.ofNullable(error.equals("") ? null : new FolderError(error));
    }

    public String concatenate()
    {
        ArrayList< Entry<TextFile, Integer> > files = new ArrayList< Entry<TextFile, Integer> >();
        files.addAll(this.files.entrySet());
        files.sort((Entry<TextFile, Integer> first, Entry<TextFile, Integer> second) -> first.getValue() < second.getValue() ? 1 : first.getValue().equals(second.getValue()) ? 0 : -1);
        String answer = "";
        for (Entry<TextFile, Integer> file : files) answer += file.getKey().getContent() + "\n";
        return answer;
    }
}
