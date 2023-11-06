/*
 * InvalidDirectoryException
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
package folder.exceptions;

import java.nio.file.Path;

/**
 * Signals that a required directory does not exist.
 * The class is a specification of {@link FolderException}.
 *
 * @version 1.0.0 2022
 * @author TTPO100AJIEX
 */
public class InvalidDirectoryException extends FolderException {

    /**
     * Constructs an {@link InvalidDirectoryException} to indicate that
     * the directory at the specified path is required by the program but
     * does not exists in the filesystem.
     * 
     * @param directory the directory the error has been detected at.
     */
    public InvalidDirectoryException(Path directory) {
        super("The folder " + directory + " has not been found!");
    }
}
