package reversi;

public final class Application {
    private Application() {
    }

    public static void main(String[] args) {
        Engine app = new Engine();
        app.run();
    }
}