package reversi.Layers;

import java.util.function.Consumer;

import reversi.Events.Event;
import reversi.Events.Control.SwitchControl;
import reversi.Events.Control.SwitchControl.Parts;
import reversi.Events.Input.InputQueryInteger;
import reversi.Events.Render.Clear;
import reversi.Events.Render.Render;
import reversi.Events.Settings.SettingsClose;
import reversi.Events.Settings.SettingsOpen;
import reversi.utils.Renderer.Renderer;

public final class Settings extends Layer implements Renderer.Item
{
    public Settings(final Consumer<Event> eventDispatcher) { super(eventDispatcher); }

    public void show()
    {
        this.eventDispatcher.accept(new SettingsOpen());
        this.eventDispatcher.accept(new Clear());
        this.eventDispatcher.accept(new Render(new Renderer.Context().add(this)));
        this.awaitAction();
        this.eventDispatcher.accept(new SettingsClose());
    }

    private void awaitAction()
    {
        InputQueryInteger action = new InputQueryInteger("Please choose the action to do: ");
        this.eventDispatcher.accept(action);
        switch(action.getValue())
        {
            case 1: { this.eventDispatcher.accept(new SwitchControl(Parts.LEADERBOARD));   break; }
            case 2: { this.eventDispatcher.accept(new SwitchControl(Parts.PLAY_PLAYER));     break; }
            case 3: { this.eventDispatcher.accept(new SwitchControl(Parts.PLAY_EASY)); break; }
            case 4: { this.eventDispatcher.accept(new SwitchControl(Parts.PLAY_ADVANCED));   break; }
            case 5: { this.eventDispatcher.accept(new SwitchControl(Parts.PLAY_RANDOM));   break; }
            case 6: { this.eventDispatcher.accept(new SwitchControl(Parts.QUIT));          break; }
            default:
            {
                Renderer.Context context = new Renderer.Context().addLine("The input is incorrect! Please, choose an integer between 1 and 6.", Renderer.Colors.RED);
                this.eventDispatcher.accept(new Render(context));
                this.awaitAction();
            }
        }
    }
    @Override public Renderer.Context getSubcontext()
    {
        return new Renderer.Context()
            .addLine("SETTINGS", Renderer.Colors.BLUE_BOLD_BRIGHT)
            
            .addText("1. ", Renderer.Colors.CYAN)
            .addLine("Leaderboard")

            .addText("2. ", Renderer.Colors.CYAN)
            .addLine("Play with other player")
        
            .addText("3. ", Renderer.Colors.CYAN)
            .addText("Play with computer (")
            .addText("easy", Renderer.Colors.GREEN)
            .addLine(" mode)")
        
            .addText("4. ", Renderer.Colors.CYAN)
            .addText("Play with computer (")
            .addText("advanced", Renderer.Colors.YELLOW)
            .addLine(" mode)")
        
            .addText("5. ", Renderer.Colors.CYAN)
            .addText("Watch two computers play (")
            .addText("random", Renderer.Colors.BLUE)
            .addLine(" mode)")
        
            .addText("6. ", Renderer.Colors.CYAN)
            .addLine("Quit");
    }
}