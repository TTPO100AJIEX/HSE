package reversi.Layers;

import reversi.Events.Event;
import reversi.Events.Input.InputQuery;
import reversi.Events.Leaderboard.LeaderboardClose;
import reversi.Events.Leaderboard.LeaderboardOpen;
import reversi.Events.Render.Clear;
import reversi.Events.Render.Render;
import reversi.utils.Pair;
import reversi.utils.Renderer.Renderer;

import java.util.ArrayList;
import java.util.function.Consumer;

public final class Leaderboard extends Layer implements Renderer.Item {
    private final ArrayList<Record> records = new ArrayList<Record>();

    public Leaderboard(final Consumer<Event> eventDispatcher) {
        super(eventDispatcher);
    }

    public void add(Record record) {
        this.records.add(record);
    }

    public void show() {
        this.eventDispatcher.accept(new LeaderboardOpen());

        this.eventDispatcher.accept(new Clear());
        this.eventDispatcher.accept(new Render(new Renderer.Context().add(this)));

        InputQuery action = new InputQuery("Press any key ot continue...");
        this.eventDispatcher.accept(action);

        this.eventDispatcher.accept(new LeaderboardClose());
    }

    @Override
    public Renderer.Context getSubcontext() {
        Renderer.Context context = new Renderer.Context().addLine("LEADERBOARD", Renderer.Colors.BLUE_BOLD_BRIGHT);
        for (Record record : this.records) context.add(record);
        return context;
    }

    public final static class Record implements Renderer.Item {
        public final int gameID;
        public final Pair<String, Integer> white, black;

        public Record(final int gameID, final Pair<String, Integer> white, final Pair<String, Integer> black) {
            this.gameID = gameID;
            this.white = white;
            this.black = black;
        }

        @Override
        public Renderer.Context getSubcontext() {
            return new Renderer.Context()
                    .addText(this.gameID + ". ", Renderer.Colors.CYAN)

                    .addText(this.white.first, Renderer.Colors.YELLOW)
                    .addText(" against ")
                    .addText(this.black.first, Renderer.Colors.PURPLE)

                    .addText(". The result was ")
                    .addText(Integer.toString(this.white.second), Renderer.Colors.CYAN)
                    .addText(":")
                    .addLine(Integer.toString(this.black.second), Renderer.Colors.CYAN);
        }
    }
}
