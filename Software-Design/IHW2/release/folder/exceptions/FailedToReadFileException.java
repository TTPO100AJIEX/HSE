/*
 * InvalidDependencyException
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
 * Signals that there was an error reading one of the required files.
 * The class is a specification of {@link FolderException}.
 *
 * @version 1.0.0 2022
 * @author TTPO100AJIEX
 */
public class FailedToReadFileException extends FolderException {

    /**
     * Constructs a {@link FailedToReadFileException} to indicate that
     * the specified file failed to be read by the programm.
     * 
     * @param file the file the error has been detected at.
     */
    public FailedToReadFileException(TextFile file) {
        super("Failed to read the contents of the file "
             + file + "! Please, try again!");
    }
}
