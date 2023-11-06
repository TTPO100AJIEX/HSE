package reversi;

import reversi.Events.Control.SwitchControl;
import reversi.Events.Control.SwitchControl.Parts;
import reversi.Events.Event;
import reversi.Events.Game.GameEnd;
import reversi.Events.Input.InputQuery;
import reversi.Events.Input.InputQueryChar;
import reversi.Events.Input.InputQueryInteger;
import reversi.Events.Input.InputQueryString;
import reversi.Events.Render.Render;
import reversi.Layers.Leaderboard;
import reversi.Layers.Reversi.ReversiGame;
import reversi.Layers.Settings;

import java.util.function.Consumer;

final class Engine {
    private final Consumer<Event> eventDispatcher;
    private final Window window;
    private final Settings settings;
    private final Leaderboard leaderboard;
    private final ReversiGame game;
    private boolean running = false;

    // for future use: multiple instances may be initiated to run multiple games in parallel (if they are run in a separate window, not in console)
    public Engine() {
        this.eventDispatcher = (Event event) -> this.event(event);
        this.window = new Window(this.eventDispatcher);
        this.settings = new Settings(this.eventDispatcher);
        this.leaderboard = new Leaderboard(this.eventDispatcher);
        this.game = new ReversiGame(this.eventDispatcher);
    }

    public void run() {
        this.window.open();
    }

    public boolean isRunning() {
        return this.running;
    }

    private void event(Event event) {
        switch (event.getEventType()) {
            case WINDOW_OPEN: {
                this.running = true;
                this.event(new SwitchControl(Parts.SETTINGS));
                break;
            }
            case WINDOW_CLOSE: {
                this.running = false;
                break;
            }

            case SETTINGS_OPEN: {
                break;
            }
            case SETTINGS_CLOSE: {
                break;
            }

            case LEADERBOARD_OPEN: {
                break;
            }
            case LEADERBOARD_CLOSE: {
                this.event(new SwitchControl(Parts.SETTINGS));
                break;
            }

            case GAME_START: {
                break;
            }
            case GAME_END: {
                this.leaderboard.add(((GameEnd) (event)).getRecord());
                this.event(new SwitchControl(Parts.SETTINGS));
                break;
            }

            case SWITCH_CONTROL: {
                this.switchControl((SwitchControl) (event));
                break;
            }

            case INPUT_QUERY_INTEGER: {
                InputQueryInteger iqievent = (InputQueryInteger) (event);
                iqievent.setValue(this.window.getInteger(iqievent.getQuery()));
                break;
            }
            case INPUT_QUERY_STRING: {
                InputQueryString iqsevent = (InputQueryString) (event);
                iqsevent.setValue(this.window.getString(iqsevent.getQuery()));
                break;
            }
            case INPUT_QUERY_CHAR: {
                InputQueryChar iqcevent = (InputQueryChar) (event);
                iqcevent.setValue(this.window.getChar(iqcevent.getQuery()));
                break;
            }
            case INPUT_QUERY: {
                InputQuery iqcevent = (InputQuery) (event);
                this.window.getInput(iqcevent.getQuery());
                break;
            }

            case CLEAR: {
                this.window.clear();
                break;
            }
            case RENDER: {
                this.window.render(((Render) (event)).getContext());
                break;
            }
        }
    }

    private void switchControl(SwitchControl event) {
        switch (event.getPart()) {
            case SETTINGS: {
                this.settings.show();
                break;
            }
            case LEADERBOARD: {
                this.leaderboard.show();
                break;
            }
            case PLAY_EASY: {
                game.run(game.new HumanPlayer(), game.new BotEasy());
                break;
            }
            case PLAY_ADVANCED: {
                game.run(game.new HumanPlayer(), game.new BotAdvanced());
                break;
            }
            case PLAY_PLAYER: {
                game.run(game.new HumanPlayer(), game.new HumanPlayer());
                break;
            }
            case PLAY_RANDOM: {
                game.run(game.new BotRandom(), game.new BotRandom());
                break;
            }
            case QUIT: {
                this.game.stop();
                this.window.close();
                break;
            }
        }
    }
}
