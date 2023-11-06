/*
 * Application
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

import java.nio.file.Paths;
import java.util.Collection;

import folder.exceptions.FolderException;

/**
 * Main class of the application.
 * The class is used to start the programm.
 * 
 * @version 1.0.0 2022
 * @author TTPO100AJIEX
 */
public final class Application {

    /**
     * The main method that runs the program.
     * The program analyzes the folder named root and prints the result:
     * <ol>
     *  <li>If any error is detected, the list of all detected errors is printed.</li>
     *  <li>If no errors have been detected, contents of all files are printed in
     * the required order.</li>
     * </ol>
     */
    public static void main() {
        /* Initialize the folder */
        Folder folder = new Folder(Paths.get("root"));
        /* Read the folder, build and check the list of dependencies */
        Collection<FolderException> errors = folder.check();
        
        if (!errors.isEmpty()) { // Something is wrong
            for (FolderException error : errors) {
                System.out.println(error.getMessage()); // Print the error
            }
            return;
        }
        System.out.println(folder.concatenate()); // Print the answer
    }

    /**
     * The main class may not be initialized,
     * so the constructor is explicitly marked as private
     */
    private Application() { }
}