package downloader;

import java.util.Scanner;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Downloader
{
    Scanner reader = new Scanner(System.in);
    String dest = "data";
    ExecutorService threadPool = Executors.newCachedThreadPool();
    public Downloader() { }

    public void run() { this.awaitCommand(); }

    private void awaitCommand()
    {
        String[] command = reader.nextLine().split(" ");
        switch (command[0])
        {
            case "/help": { this.help(command); break; }
            case "/load": { this.load(command); break; }
            case "/dest": { this.dest(command); break; }
            case "/exit": { this.exit(command); return; }
            default: { System.out.println("Unknown command!"); }
        }
        this.awaitCommand();
    }

    private void help(String[] command)
    {
        System.out.println("/help - показ всех команд и справки\n" +
                            "/load URL - загрузка файла по урлу\n" + 
                            "/load URL1 URL2 URL3 и т.д - загрузка файлов по урлам\n" +
                            "/dest PATH указать путь куда скачивать файлы\n" +
                            "/exit - выход");
    }

    private void dest(String[] command)
    {
        this.dest = command[1];
    }

    private void load(String[] command)
    {
        for (int i = 1; i < command.length; i++)
        {
            this.threadPool.execute(new FileDownload(command[i], this.dest));
            System.out.println("Scheduled " + command[i]);
        }
    }

    private void exit(String[] command)
    {
        this.reader.close();
        this.threadPool.shutdown();
        while (!this.threadPool.isTerminated()) { }
    }
}
