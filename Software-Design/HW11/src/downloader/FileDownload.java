package downloader;

import java.io.IOException;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.Paths;

public class FileDownload extends Thread
{
    String url, dest;
    public FileDownload(String url, String dest)
    {
        this.url = url;
        this.dest = dest;
    }

    @Override
    public void run()
    {
        try
        {
            Files.copy(new URL(this.url).openStream(), Paths.get(dest + "/" + this.url.substring(this.url.lastIndexOf("/"))));
            System.out.println("Downloaded " + this.url);
        } catch(IOException err) { System.out.println("Failed to download " + this.url); }
    }
}
