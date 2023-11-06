/*
 * Folder
 * v1.0.0
 * Copyright (c) 2022 TTPO100AJIEX
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
package folder;

import java.nio.file.DirectoryStream;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.Optional;
import java.util.Map.Entry;

import folder.exceptions.CircularDependencyException;
import folder.exceptions.FailedToReadFileException;
import folder.exceptions.FolderException;
import folder.exceptions.InvalidDependencyException;
import folder.exceptions.InvalidDirectoryException;

/**
 * A class to describe and analyze a folder.
 * The class implements a storage of all files in the folder and its
 * subfolders. The files in the folder can be sorted topologically accounting
 * the dependencies. If such ordering is not possible, the class allows for
 * getting the list of all detected errors.
 * The class relies on {@link TextFile} to parse the contents of the text file
 * and get its dependencies.
 * 
 * @version 1.0.0 2022
 * @author TTPO100AJIEX
 */
public final class Folder {

    /**
     * Constant required for the topological sort implementation.
     * The code indicates that the current vertex (file) has been entered by
     * the dfs implementation, but has not yet been exited.
     */
    private static final int VERTEX_ENTERED = -2;

    /**
     * The code indicates that the file participates in a cirular dependency.
     * If the vertex (file) is marked with this code after the topological
     * sort, an exception must be generated.
     */
    private static final int VERTEX_CIRCULAR = -3;

    /**
     * The code indicates that the file has an invalid dependency - the file
     * that does not exist in the filesystem. If the vertex (file) is marked
     * with this code after the topological sort, an exception must be
     * generated.
     */
    private static final int VERTEX_INVALID = -4;

    /**
     * The path to the folder relative to the executable.
     * May not be changed after the instance is created.
     */
    private final Path path;

    /**
     * Counter required for the topological sort implementation.
     * Stores the index that must be assigned to the next file in the order
     * when it is determined.
     * The value of -1 indicates that the order has not been calculated yet
     * and must be calculated whenever it is needed.
     * The value of 0 indicates that the stored order is up-to-date and can be
     * used without recalculation.
     */
    private int calculteOrderingNextIndex = -1;

    /**
     * The storage of all files in this folder and its subfolders.
     * Each file has a corresponding Integer - the status of the file:
     * <ol>
     *  <li>positive values indicate the topological order of the file after it
     * has been calculated.</li>
     *  <li>-1 indicates an undefined state of the file: the only available piece
     * of information is that the file exists. Nothing can be determined about
     * its correctness and order.</li>
     *  <li>-2 indicates that the vertex has been entered
     * by the dfs calculating the topological sort. </li>
     *  <li>-3 indicates that the file participates in a
     * circular dependency</li>
     *  <li>-4 indicates that the file has an invalid
     * dependency - requires the file that does not exist.</li>
     * </ol>
     * @see VERTEX_ENTERED
     * @see VERTEX_CIRCULAR
     * @see VERTEX_INVALID
     */
    private HashMap<TextFile, Integer> files = new HashMap<>();

    /**
     * Initializes a new folder at the specified path.
     * 
     * @param path the path to the directory relative to the executable
     */
    public Folder(Path path) {
        this.path = path;
    }

    /**
     * Scans the specified directory and saves all found files.
     * If any file failed to be read or any required directory is invalid,
     * a respective exception is thrown.
     * The implementation relies on {@link DirectoryStream}, {@link Files} APIs
     * of the common java library.
     * All found files are saved in the {@link files} data structure.
     * 
     * @param dirpath the path to the directory to scan
     * @throws InvalidDirectoryException if the required directory does not
     * exist in the filsystem
     * @throws FailedToReadFileException if any file could not have been read
     */
    private void searchFiles(Path dirpath)
        throws InvalidDirectoryException, FailedToReadFileException {
        try (DirectoryStream<Path> directory =
                Files.newDirectoryStream(dirpath)) {
            for (Path filename : directory) {
                if (Files.isDirectory(filename)) {
                    this.searchFiles(filename);
                } else {
                    this.files.put(new TextFile(filename), -1);
                }
            }
        } catch(Exception e) {
            throw new InvalidDirectoryException(dirpath);
        }
    }

    /**
     * Scans the directory and its subdirectories and saves all found files.
     * If the directory has already been scanned, the process is not repeated.
     * If any file failed to be read or any required directory is invalid,
     * a respective exception is thrown.
     * 
     * @throws InvalidDirectoryException if the required directory does not
     * exist in the filsystem
     * @throws FailedToReadFileException if any file could not have been read
     */
    private void searchFiles()
        throws InvalidDirectoryException, FailedToReadFileException {
        if (!this.files.isEmpty()) {
            return;
        }
        this.searchFiles(this.path);
    }
    
    /**
     * The recursive implementation of dfs for topological sorting.
     * The function analyzes the file passed as the sole argument and sets its
     * status in the {@link files} member variable accordingly, recursively
     * calling itself on all dependencies of the file being analyzed.
     * If the ordering cannot be determined, a respective error status is set.
     * Otherwise, the status is set to be equal to the index of the file in the
     * sorted order.
     * 
     * @param now the file being analyzed with this call.
     */
    private void calculteOrdering(Entry<TextFile, Integer> now) {
        if (now.getValue() == Folder.VERTEX_ENTERED) {
            /* This vertex is entered => loop detected */
            now.setValue(Folder.VERTEX_CIRCULAR);
            return;
        }
        if (now.getValue() < Folder.VERTEX_ENTERED || now.getValue() != -1) {
            /* This vertex has been visited and exited */
            return;
        }
        now.setValue(Folder.VERTEX_ENTERED); // Enter the vertex
        for (String file : now.getKey().getDependencies()) {
            Path filepath = Paths.get(this.path.toString(), file);
            Optional< Entry<TextFile, Integer> > fileObject = 
                this.files.entrySet().stream()
                    .filter((Entry<TextFile, Integer> entry) ->
                            entry.getKey().equals(filepath))
                    .findFirst();
            if (!fileObject.isPresent())
            {
                /* Invalid dependency detected */
                now.setValue(Folder.VERTEX_INVALID);
                continue;
            }
            int storedValue = fileObject.get().getValue();
            this.calculteOrdering(fileObject.get());
            if ((storedValue != Folder.VERTEX_CIRCULAR)
                && (fileObject.get().getValue() == Folder.VERTEX_CIRCULAR)) {
                now.setValue(Folder.VERTEX_CIRCULAR);
            }
        }
        if (now.getValue() != Folder.VERTEX_ENTERED) {
            return;
        }
        now.setValue(this.calculteOrderingNextIndex--);
    }

    /**
     * Calculates the topological ordering of the files accounting the
     * dependencies using recursive dfs. The ordering is stored in the
     * {@link files} member variable. If a circular or an invalid dependency
     * is detected, a respective status is set to the problematic file.
     * If the ordering has already been calculated and is up-to-date, the
     * function does not do anything.
     */
    private void calculteOrdering() {
        if (this.files.isEmpty() || (this.calculteOrderingNextIndex != -1)) {
            return;
        }
        this.calculteOrderingNextIndex = this.files.size();
        for (Entry<TextFile, Integer> file : this.files.entrySet()) {
            this.calculteOrdering(file);
        }
    }

    /**
     * Checks whether the topological ordering of the files in this folder and
     * its subfolders is possible. 
     * If the folder has not yet been scanne or sorted, those operations are
     * executed before the analysis.
     * 
     * @return An {@link ArrayList} of {@link FolderException} containing
     * descriptions of all errors that have been detected. If no errors have
     * been found, an empty array is returned.
     */
    public Collection<FolderException> check()
    {
        ArrayList<FolderException> errors = new ArrayList<>();

        try {
            this.searchFiles();
        } catch(InvalidDirectoryException e) {
            errors.add(e);
            return errors;
        } catch(FailedToReadFileException e) {
            errors.add(e);
            return errors;
        }

        this.calculteOrdering();
        for (Entry<TextFile, Integer> file : this.files.entrySet()) {
            switch (file.getValue()) {
            case Folder.VERTEX_INVALID:
                errors.add(new InvalidDependencyException(file.getKey()));
                break;
                
            case Folder.VERTEX_CIRCULAR:
                errors.add(new CircularDependencyException(file.getKey()));
                break;

            case Folder.VERTEX_ENTERED, -1, 0:
                errors.add(new FolderException(file.getKey()));
                break;

            default:
                /* falls through */
            }
        }
        return errors;
    }

    /**
     * Concatenates the contents of the files in the topological order.
     * It is required to call {@link check} prior to the call to this method.
     * Otherwise, the behavior is undefined. 
     * 
     * If the read access on any file has disappeared during the worktime of
     * the program (between the check call and this call), the contents of the
     * respectie are will be replaced with an empty string.
     * 
     * @return A string representing the concatenated contents of all files in
     * this folder and its subfolders in the topological order.
     */
    public String concatenate() {
        ArrayList< Entry<TextFile, Integer> > textFiles = 
                new ArrayList<>(this.files.entrySet());
        String answer = "";
        
        textFiles.sort((Entry<TextFile, Integer> first,
                        Entry<TextFile, Integer> second) ->
            (first.getValue() < second.getValue()) ? 1
                : (first.getValue().equals(second.getValue()) ? 0 : -1)
        );
        for (Entry<TextFile, Integer> file : textFiles) {
            answer += file.getKey().getContent() + "\n";
        }
        return answer;
    }
}
