/*
 * TextFile
 * v1.0.0
 * Copyright 2022 TTPO100AJIEX
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
package folder;

import java.nio.file.Path;
import java.nio.file.Files;
import java.io.File;
import java.util.Scanner;
import java.util.Collection;
import java.util.HashSet;

import folder.exceptions.FailedToReadFileException;

/**
 * A class to describe and analyze a text files.
 * Two text files are considered equal if they are stored under the same path.
 * The files may be stored in a {@link HashSet} or {@link HashMap}.
 * 
 * @version 1.0.0 2022
 * @author TTPO100AJIEX
 */
public final class TextFile {

    /**
     * The path under which the file is stored.
     * May not be changed after the instance is created.
     * Used as a key for {@link HashSet} and {@link HashMap}:
     * @see equals
     * @see hashCode
     */
    private final Path path;

    /**
     * The list of dependencies of the file.
     * Initialized at the construction time by parsing all require directives
     * in the contents of the file.
     * @see TextFile
     */
    private HashSet<String> dependencies = new HashSet<>();

    /**
     * Initializes a new text file at the specified path.
     * If the file could not have been read, a
     * {@link FailedToReadFileException} is thrown.
     * Relies on {@link Scanner} and {@link File}.
     * 
     * @param path the path to the file relative to the executable
     * @throws FailedToReadFileException if the file could not have been read
     */
    public TextFile(Path path) throws FailedToReadFileException {
        this.path = path;
        try (Scanner reader = new Scanner(new File(path.toString()))) {
            while (reader.hasNextLine()) {
                String line = reader.nextLine();
                if ((!line.startsWith("require '"))
                        || (!line.endsWith("'"))) {
                    continue;
                }
                dependencies.add(line.substring(line.indexOf("'") + 1,
                                                line.lastIndexOf("'")));
            }
        } catch(Exception e) {
            throw new FailedToReadFileException(this);
        }
    }

    /**
     * Returns a {@link Collection} of the dependencies of the current file.
     * Each dependency is a string that represents the path of the dependency
     * file relative to the root folder.
     * 
     * @return The list of the dependencies of the current file
     */
    public Collection<String> getDependencies() {
        return this.dependencies;
    }

    /**
     * Returns the contents of the current file as a single string.
     * If the file could not have been read, an empty string is returned.
     * 
     * @return The contents of the text file if it was read successfully.
     * @return An empty string if the text file was not read successfully.
     */
    public String getContent() {
        try {
            return Files.readString(this.path);
        } catch(Exception e) {
            return "";
        }
    }

    /**
     * Checks whether two files are equal.
     * Two files are considered equal if their paths are equal.
     * 
     * @param other the {@link Object} to compare to.
     * @return true if the files are equal; false if the files are not equal or {@code other} is not a file.
     */
    @Override public boolean equals(Object other) {
        return (
                (other instanceof TextFile) && 
                (((TextFile) (other)).path.equals(this.path))
        );
    }

    /**
     * Checks whether the current file is located at the specified path.
     * That is, checks if {@code this.equals(new TextFile(other))}.
     * @see equals
     * 
     * @param other the {@link Path} to compare to.
     * @return true if the files are equal.
     * @return false if the files are not equal or other is not a file.
     */
    public boolean equals(Path other) {
        return other.equals(this.path);
    }

    /**
     * Hashes the current object and returns the hash.
     * May be used by {@link HashSet} and {@link HashMap}.
     * For generating the hash relies on the implementation of {@link Path::hashCode}
     * 
     * @return hash of the current text file
     */
    @Override public int hashCode() {
        return this.path.hashCode();
    }

    /**
     * Returns the string representation of the text file.
     * That is, the path to the file as it was passed to the constructor.
     * 
     * @return string representation of the path to the file.
     */
    @Override public String toString() {
        return this.path.toString();
    }
}
