/*
 * FolderException
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

import folder.TextFile;


/**
 * Signals that an exception of some sort has occurred at runtime.
 * General class for exceptions that may occur when analyzing the folder.
 * 
 * Some common types of errors extend this class for convenient usage.
 * @see CircularDependencyException
 * @see FailedToReadFileException
 * @see InvalidDependencyException
 * @see InvalidDirectoryException
 *
 * @version 1.0.0 2022
 * @author TTPO100AJIEX
 */
public class FolderException extends Exception {
    
    /**
     * Constructs a {@link FolderException} with the specified message.
     * 
     * @param description the description of the exception
     */
    public FolderException(String description) {
        super(description);
    }

    /**
     * Constructs a {@link FolderException} as an unknown error on the
     * specified text file.
     * 
     * @param file the file an exception has occurred at.
     */
    public FolderException(TextFile file) {
        super("An unknown error has occurred on "
             + file + "! Please, try again!\n");
    }
}
