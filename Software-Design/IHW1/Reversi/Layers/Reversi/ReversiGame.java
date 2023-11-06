package reversi.Layers.Reversi;

import java.util.ArrayList;
import java.util.Random;
import java.util.function.Consumer;

import reversi.Events.Event;
import reversi.Events.Game.GameEnd;
import reversi.Events.Input.InputQueryChar;
import reversi.Events.Input.InputQueryInteger;
import reversi.Events.Input.InputQueryString;
import reversi.Events.Render.Clear;
import reversi.Events.Render.Render;
import reversi.Layers.Leaderboard;
import reversi.utils.Pair;
import reversi.utils.Renderer.Renderer;

public class ReversiGame extends Reversi
{
    /*------------------------------------------------------------*/
    /*---------------------------Player---------------------------*/
    /*------------------------------------------------------------*/
    public abstract class Player
    {
        protected final String name;
        protected Player(final String name) { this.name = name; }
        protected Player()
        {
            InputQueryString QueryName = new InputQueryString("Please enter the name of the player: ");
            ReversiGame.this.eventDispatcher.accept(QueryName);
            this.name = QueryName.getValue();
        }
        public abstract Field.Move genMove();
    }
    public class HumanPlayer extends Player
    {
        public HumanPlayer(final String name) { super(name); }
        public HumanPlayer() { super(); }
        
        @Override public Field.Move genMove()
        {
            InputQueryInteger row_input = new InputQueryInteger("Please enter the coordinates for the new piece (e.g. 1 a). To cancel the last move please enter -1...\n");
            ReversiGame.this.eventDispatcher.accept(row_input);
            if (row_input.getValue() == -1) return null;
            InputQueryChar column_input = new InputQueryChar("");
            ReversiGame.this.eventDispatcher.accept(column_input);
            return ReversiGame.this.field.new Move(row_input.getValue() - 1, column_input.getValue() - 'a');
        }
    }
    public class BotRandom extends Player
    {
        public BotRandom(final String name) { super(name); }
        public BotRandom() { super("BotRandom"); }
        
        @Override public Field.Move genMove()
        {
            ArrayList<Field.Move> available_moves = new ArrayList<Field.Move>();
            for (Field.Iterator it = ReversiGame.this.field.begin(); it.isValid(); it.next())
            {
                if (it.cell().isAvailable()) available_moves.add(ReversiGame.this.field.new Move(it));
            }
            try { Thread.sleep(750); } catch(Exception err) { }
            return available_moves.get(new Random().nextInt(available_moves.size()));
        }
    }
    public class BotEasy extends Player
    {
        public BotEasy(final String name) { super(name); }
        public BotEasy() { super("BotEasy"); }
        
        @Override public Field.Move genMove()
        {
            Field.Move move = ReversiGame.this.emulate().findBestMove(1).second;
            try { Thread.sleep(1250); } catch(Exception err) { }
            return ReversiGame.this.field.new Move(move);
        }
    }
    public class BotAdvanced extends Player
    {
        public BotAdvanced(final String name) { super(name); }
        public BotAdvanced() { super("BotAdvanced"); }
        
        @Override public Field.Move genMove()
        {
            Field.Move move = ReversiGame.this.emulate().findBestMove(2).second;
            try { Thread.sleep(1250); } catch(Exception err) { }
            return ReversiGame.this.field.new Move(move);
        }
    }
    
    /*----------------------------------------------------------*/
    /*---------------------------GAME---------------------------*/
    /*----------------------------------------------------------*/
    private static int nextGameID = 0;
    private int gameID = 0;
    public ReversiGame(final Consumer<Event> eventDispatcher)
    {
        super(eventDispatcher);
    }
    
    private boolean running = false;
    private Player white, black;
    public void run(final Player white, final Player black)
    {
        this.stop();
        this.field = new Field(8, 8);
        this.gameID = ++ReversiGame.nextGameID;
        this.white = white;
        this.black = black;
        this.isBlackMove = false;
        this.running = true;
        this.nextMove();
    }
    public Player getCurrentPlayer() { return this.isBlackMove ? this.black : this.white; }
    public void stop() { if (this.running) this.end(); }
    
    private int consecutiveNoMovesAmount = 0;
    private void nextMove()
    {
        if (!this.running) return;
        this.renderField();
        this.renderActivePlayer();
        if (!this.hasAvailableMoves())
        {
            this.renderNoMoves();
            this.consecutiveNoMovesAmount++;
            try { Thread.sleep(2500); } catch(Exception err) { }
            if (this.consecutiveNoMovesAmount >= 2) { this.end(); return; }
        }
        else
        {
            this.consecutiveNoMovesAmount = 0;
            this.renderAvailableMoves();
            this.handleMove();
        }

        this.isBlackMove = !this.isBlackMove;
        this.nextMove();
    }
    private void end()
    {
        this.running = false;
        Leaderboard.Record record = new Leaderboard.Record(this.gameID, new Pair<String, Integer>(this.white.name, this.whiteScore()), new Pair<String, Integer>(this.black.name, this.blackScore()));
        if (this.whiteScore() == this.blackScore()) this.renderDraw();
        if (this.whiteScore() > this.blackScore()) this.renderVictoryFirst();
        if (this.whiteScore() < this.blackScore()) this.renderVictorySecond();
        try { Thread.sleep(5000); } catch(Exception err) { }
        this.eventDispatcher.accept(new GameEnd(record));
    }
    private int whiteScore()
    {
        int score = 0;
        for (Field.Iterator it = this.field.begin(); it.isValid(); it.next())
        {
            if (it.cell().isWhite()) score++;
        }
        return score;
    }
    private int blackScore()
    {
        int score = 0;
        for (Field.Iterator it = this.field.begin(); it.isValid(); it.next())
        {
            if (it.cell().isBlack()) score++;
        }
        return score;
    }
    
    /*---------------------------Render---------------------------*/
    private void renderField()
    {
        this.eventDispatcher.accept(new Clear());
        this.eventDispatcher.accept(new Render(new Renderer.Context().add(this.field)));
        this.field.resetMarks();
    }
    private void renderActivePlayer()
    {
        Renderer.Context context = new Renderer.Context().addText("The move of ").addText(this.getCurrentPlayer().name, Renderer.Colors.GREEN_UNDERLINED).addLine(" (" + (this.isBlackMove ? "B" : "W") + ")!");
        this.eventDispatcher.accept(new Render(context));
    }
    
    private boolean hasAvailableMoves()
    {
        for (Field.Iterator it = this.field.begin(); it.isValid(); it.next())
        {
            if (it.cell().isAvailable()) return true;
        }
        return false;
    }
    private void renderNoMoves()
    {
        Renderer.Context context = new Renderer.Context().addText("Available moves: ", Renderer.Colors.PURPLE).addLine("no moves are available", Renderer.Colors.RED);
        this.eventDispatcher.accept(new Render(context));
    }
    private void renderAvailableMoves()
    {
        Renderer.Context context = new Renderer.Context().addText("Available moves: ", Renderer.Colors.PURPLE);
        for (Field.Iterator it = this.field.begin(); it.isValid(); it.next())
        {
            if (it.cell().isAvailable()) context.addText(it.cell().toString(), Renderer.Colors.CYAN).addText(", ");
        }
        context.remove(-1).addLine();
        this.eventDispatcher.accept(new Render(context));
    }

    /*---------------------------Move---------------------------*/
    private ArrayList<ReversiEmulator> history = new ArrayList<ReversiEmulator>();
    private void handleMove()
    {
        Field.Move move = this.getCurrentPlayer().genMove();
        if (move == null) { this.cancelMove(); this.isBlackMove = !this.isBlackMove; return; }
        while (!move.isValid())
        {
            Renderer.Context context = new Renderer.Context().addText("The move " + move.toString() + " is not allowed!", Renderer.Colors.RED).addLine(" Please specify a valid move!", Renderer.Colors.YELLOW);
            this.eventDispatcher.accept(new Render(context));
            move = this.getCurrentPlayer().genMove();
            if (move == null) { this.cancelMove(); this.isBlackMove = !this.isBlackMove; return; }
        }
        if (this.getCurrentPlayer().getClass() == HumanPlayer.class) this.history.add(this.emulate());
        move.register();
    }
    private void cancelMove()
    {
        if (this.history.isEmpty()) return;
        this.clone(this.history.get(this.history.size() - 1));
        this.history.remove(this.history.size() - 1);
    }
    

    /*------------------------------------------------------------*/
    /*---------------------------Result---------------------------*/
    /*------------------------------------------------------------*/
    private interface PlayerResult extends Renderer.Item
    {
        public String getStatus();
        public Renderer.Colors getStatusColor();

        public String getPlayerName();
        public Renderer.Colors getPlayerColor();
        
        public int getScore();

        @Override public default Renderer.Context getSubcontext()
        {
            return new Renderer.Context()
                .addText("The ")    .addText(this.getStatus(), this.getStatusColor())
                .addText(" is ")    .addText(this.getPlayerName(), this.getPlayerColor())
                .addText(" with a ").addText("score", Renderer.Colors.CYAN)
                .addText(" of ")    .addLine(Integer.toString(this.getScore()), Renderer.Colors.BLUE);
        }
    }
    private final class Winner implements PlayerResult
    {
        private final Player player;
        public Winner(Player player) { this.player = player; }

        @Override public String getStatus() { return "winner"; }
        @Override public Renderer.Colors getStatusColor() { return Renderer.Colors.GREEN; }

        @Override public String getPlayerName() { return this.player.name; }
        @Override public Renderer.Colors getPlayerColor() { return (ReversiGame.this.white == this.player) ? Renderer.Colors.YELLOW : Renderer.Colors.PURPLE; }
        
        @Override public int getScore() { return (ReversiGame.this.white == this.player) ? ReversiGame.this.whiteScore() : ReversiGame.this.blackScore(); }
    }
    private final class Loser implements PlayerResult
    {
        private final Player player;
        public Loser(Player player) { this.player = player; }

        @Override public String getStatus() { return "loser"; }
        @Override public Renderer.Colors getStatusColor() { return Renderer.Colors.RED; }

        @Override public String getPlayerName() { return this.player.name; }
        @Override public Renderer.Colors getPlayerColor() { return (ReversiGame.this.white == this.player) ? Renderer.Colors.YELLOW : Renderer.Colors.PURPLE; }
        
        @Override public int getScore() { return (ReversiGame.this.white == this.player) ? ReversiGame.this.whiteScore() : ReversiGame.this.blackScore(); }
    }
    private void renderDraw()
    {
        Renderer.Context context = new Renderer.Context().addLine().addLine("THERE ARE NO AVAILABLE MOVES", Renderer.Colors.PURPLE);
        context.addText("The winner has not been determined! The game is considered a ").addText("draw", Renderer.Colors.YELLOW).addLine("!");
        this.eventDispatcher.accept(new Render(context));
    }
    private void renderVictoryFirst()
    {
        Renderer.Context context = new Renderer.Context()
            .addLine().addLine("THERE ARE NO AVAILABLE MOVES", Renderer.Colors.BLUE_BOLD_BRIGHT)
            .add(new Winner(this.white)).add(new Loser(this.black))
            .addLine("You will be redirected to the menu...");
        this.eventDispatcher.accept(new Render(context));
    }
    private void renderVictorySecond()
    {
        Renderer.Context context = new Renderer.Context()
            .addLine().addLine("THERE ARE NO AVAILABLE MOVES", Renderer.Colors.BLUE_BOLD_BRIGHT)
            .add(new Winner(this.black)).add(new Loser(this.white))
            .addLine("You will be redirected to the menu...");
        this.eventDispatcher.accept(new Render(context));
    }
}
