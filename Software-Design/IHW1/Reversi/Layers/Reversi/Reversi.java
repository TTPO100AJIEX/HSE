package reversi.Layers.Reversi;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashSet;
import java.util.Set;
import java.util.function.Consumer;

import reversi.Events.Event;
import reversi.Layers.Layer;
import reversi.utils.Renderer.Renderer;

public abstract class Reversi extends Layer
{
    protected boolean isBlackMove = false; // false - player1 (W), true - player2 (B)
    protected Field field;
    protected Reversi(final Consumer<Event> eventDispatcher)
    {
        super(eventDispatcher);
    }
    
    protected ReversiEmulator emulate() { return new ReversiEmulator(this); }
    protected void merge(final Reversi emulator) { this.field = emulator.field; this.isBlackMove = emulator.isBlackMove; }
    protected void clone(final Reversi emulator) { this.field = new Field(emulator.field); this.isBlackMove = emulator.isBlackMove; }
    
    protected class Field implements Renderer.Item
    {
        /*----------------------------------------------------------*/
        /*---------------------------Cell---------------------------*/
        /*----------------------------------------------------------*/
        protected class Cell implements Renderer.Item
        {
            private enum Status { EMPTY, WHITE, BLACK };
            private enum Mark { NONE, PAINTED, UPDATED };
            
            private final int row, column;
            private Status status = Status.EMPTY;
            private Mark mark = Mark.NONE;
            public Cell(final int row, final int column) { this.row = row; this.column = column; }
            public Cell(final Cell other) { this.row = other.row; this.column = other.column; this.status = other.status; this.mark = other.mark; }
            
            public int row()    { return this.row; }
            public int column() { return this.column; }
            public boolean equals(final Cell other) { return (this.row == other.row && this.column == other.column); }

            public boolean isEmpty() { return this.status == Status.EMPTY; }
            public boolean isWhite() { return this.status == Status.WHITE; }
            public boolean isBlack() { return this.status == Status.BLACK; }
            public boolean isPainted() { return this.mark == Mark.PAINTED; }
            public boolean isUpdated() { return this.mark == Mark.UPDATED; }
            public boolean isAvailable() { return (this.isEmpty() && this.isNearEnemy() && !this.getEnclosure().isEmpty()); }
            public boolean isCorner() { return((this.row == 0 || this.row == Field.this.getHeight() - 1) && (this.column == 0 || this.column == Field.this.getWidth() - 1)); }
            public boolean isEdge() { return(this.row == 0 || this.row == Field.this.getHeight() - 1 || this.column == 0 || this.column == Field.this.getWidth() - 1); }
            public boolean isNearEnemy()
            {
                boolean IsBlackMove = Reversi.this.isBlackMove;
                for (int i = -1; i <= 1; i++)
                {
                    for (int j = -1; j <= 1; j++)
                    {
                        if (IsBlackMove && Field.this.get(this.row + i, this.column + j).cell().isWhite()) return true;
                        if (!IsBlackMove && Field.this.get(this.row + i, this.column + j).cell().isBlack()) return true;
                    }
                }
                return false;
            }

            public Collection<Cell> getEnclosure()
            {
                Set<Field.Cell> answer = new HashSet<Field.Cell>();
                answer.addAll(this.getEnclosure(Field.this.new VerticalIterator(this)));
                answer.addAll(this.getEnclosure(Field.this.new HorizontalIterator(this)));
                answer.addAll(this.getEnclosure(Field.this.new PrimaryDiagonalIterator(this)));
                answer.addAll(this.getEnclosure(Field.this.new SecondaryDiagonalIterator(this)));
                answer.addAll(this.getEnclosure(Field.this.new ReverseVerticalIterator(this)));
                answer.addAll(this.getEnclosure(Field.this.new ReverseHorizontalIterator(this)));
                answer.addAll(this.getEnclosure(Field.this.new ReversePrimaryDiagonalIterator(this)));
                answer.addAll(this.getEnclosure(Field.this.new ReverseSecondaryDiagonalIterator(this)));
                return answer;
            }
            public Collection<Cell> getEnclosure(Iterator iter)
            {
                Field.Iterator initial = Field.this.new Iterator(iter);
                iter.next();
                if (Reversi.this.isBlackMove) { while (iter.cell().isWhite()) iter.next(); }
                else { while (iter.cell().isBlack()) iter.next(); }
                
                ArrayList<Field.Cell> answer = new ArrayList<Field.Cell>();
                if (iter.cell().isEmpty()) return answer; // no end of enclosure
                iter.previous();
                for ( ; !iter.equals(initial); iter.previous()) answer.add(iter.cell());
                return answer;
            }
            
            public void setBlack() { this.status = Status.BLACK; this.mark = Mark.PAINTED; }
            public void setWhite() { this.status = Status.WHITE; this.mark = Mark.PAINTED; }
            public void changeColor() { this.status = (this.status == Status.WHITE ? Status.BLACK : Status.WHITE); this.mark = Mark.UPDATED; }
            public void resetMark() { this.mark = Mark.NONE; }
            
            @Override public String toString() { return ((this.row + 1) + " " + (char)(this.column + 'a')); }
        
            /*------------------------Renderer.Item------------------------*/
            @Override public String getData()
            {
                if (this.isAvailable()) return " A ";
                switch (this.status)
                {
                    case WHITE: return " W ";
                    case BLACK: return " B ";
                    default:    return "   ";
                }
            }
            @Override public Renderer.Colors getColor()
            {
                if (this.isAvailable()) return Renderer.Colors.BLUE_BOLD_BRIGHT;
                if (this.isUpdated())   return Renderer.Colors.BLUE_BACKGROUND;
                if (this.isPainted())   return Renderer.Colors.GREEN_BACKGROUND;
                if (this.isWhite())     return Renderer.Colors.YELLOW_BACKGROUND;
                if (this.isBlack())     return Renderer.Colors.PURPLE_BACKGROUND;
                return Renderer.Colors.NONE;
            }
        }
        
        /*-----------------------------------------------------------*/
        /*---------------------------Field---------------------------*/
        /*-----------------------------------------------------------*/
        private Cell[][] field;
        public Field(final int width, final int height)
        {
            this.field = new Cell[width][height];
            for (int i = 0; i < width; i++) for (int j = 0; j < height; j++) this.field[i][j] = new Cell(i, j);
            this.field[this.getHeight() / 2 - 1][this.getWidth() / 2 - 1].setWhite(); this.field[this.getHeight() / 2][this.getWidth() / 2].setWhite();
            this.field[this.getHeight() / 2][this.getWidth() / 2 - 1].setBlack(); this.field[this.getHeight() / 2 - 1][this.getWidth() / 2].setBlack();
            this.resetMarks();
        }
        public Field(final Field other)
        {
            this.field = new Cell[other.getWidth()][other.getHeight()];
            for (int i = 0; i < other.getWidth(); i++) for (int j = 0; j < other.getHeight(); j++) this.field[i][j] = new Cell(other.field[i][j]);
        }
        
        public Iterator begin() { return new Iterator(0, 0); }
        public Iterator get(final int i, final int j) { return new Iterator(i, j); }
        public int getWidth()  { return this.field.length; }
        public int getHeight() { return this.field[0].length; }

        public void resetMarks()
        {
            for (Iterator it = this.begin(); it.isValid(); it.next()) it.cell().resetMark();
        }
        
        /*-----------------------Renderer.Item-----------------------*/
        @Override public Renderer.Context getSubcontext()
        {
            final int height_length = Integer.toString(this.getHeight()).length();
            final int LINE_LENGTH = 7 + height_length + 4 * this.getWidth() + 2;
            Renderer.Context context = new Renderer.Context().addLine("-".repeat(LINE_LENGTH)).addText("|" + " ".repeat(height_length + 3) + "|||");
            for (int j = 0; j < this.getWidth(); j++)
            {
                context.addText(" " + (char)('a' + j) + " ", Renderer.Colors.CYAN);
                if (j == this.getWidth() - 1) context.addLine("|||");
                else context.addText("|");
            }
            context.addLine("-".repeat(LINE_LENGTH));
            for (int i = 0; i < this.getHeight(); i++)
            {
                String number = Integer.toString(i + 1);
                number = " ".repeat(height_length - number.length()) + number;
                context.addText("| ").addText(number + ".", Renderer.Colors.CYAN).addText(" |||");
                for (int j = 0; j < this.getWidth(); j++)
                {
                    context.add(this.field[i][j]);
                    if (j == this.getWidth() - 1) context.addLine("|||");
                    else context.addText("|");
                }
                context.addLine("-".repeat(LINE_LENGTH));
            }
            return context;
        }

        /*----------------------------------------------------------*/
        /*-------------------------Iterator-------------------------*/
        /*----------------------------------------------------------*/
        public class Iterator
        {
            protected int row, column;
            public Iterator(final int row, final int column) { this.row = row; this.column = column; }
            public Iterator(final Cell cell) { this.row = cell.row(); this.column = cell.column(); }
            public Iterator(final Iterator other) { this.row = other.row(); this.column = other.column(); }

            public int row() { return this.row; }
            public int column() { return this.column; }
            public boolean isValid() { return (this.row >= 0 && this.row < Field.this.getHeight() && this.column >= 0 && this.column < Field.this.getWidth()); }
            public Cell cell()
            {
                if (!this.isValid()) return new Cell(-1, -1);
                return Field.this.field[this.row][this.column];
            }

            public void advance(final int offset)
            {
                int index = this.row * Field.this.getWidth() + this.column + offset;
                this.row = index / Field.this.getWidth(); this.column = index % Field.this.getWidth();
            }
            public void next() { this.advance(1); }
            public void previous() { this.advance(-1); }
    
            public boolean equals(final Iterator other) { return (this.row == other.row() && this.column == other.column()); }
        }
        public class VerticalIterator extends Iterator
        {
            public VerticalIterator(final int row, final int column) { super(row, column); }
            public VerticalIterator(final Cell cell) { super(cell); }
            public VerticalIterator(final Iterator other) { super(other); }
            @Override public void advance(final int offset) { this.row += offset; }
        }
        public class HorizontalIterator extends Iterator
        {
            public HorizontalIterator(final int row, final int column) { super(row, column); }
            public HorizontalIterator(final Cell cell) { super(cell); }
            public HorizontalIterator(final Iterator other) { super(other); }
            @Override public void advance(final int offset) { this.column += offset; }
        }
        public class PrimaryDiagonalIterator extends Iterator
        {
            public PrimaryDiagonalIterator(final int row, final int column) { super(row, column); }
            public PrimaryDiagonalIterator(final Cell cell) { super(cell); }
            public PrimaryDiagonalIterator(final Iterator other) { super(other); }
            @Override public void advance(final int offset) { this.row += offset; this.column += offset; }
        }
        public class SecondaryDiagonalIterator extends Iterator
        {
            public SecondaryDiagonalIterator(final int row, final int column) { super(row, column); }
            public SecondaryDiagonalIterator(final Cell cell) { super(cell); }
            public SecondaryDiagonalIterator(final Iterator other) { super(other); }
            @Override public void advance(final int offset) { this.row += offset; this.column -= offset; }
        }
        public class ReverseVerticalIterator extends Iterator
        {
            public ReverseVerticalIterator(final int row, final int column) { super(row, column); }
            public ReverseVerticalIterator(final Cell cell) { super(cell); }
            public ReverseVerticalIterator(final Iterator other) { super(other); }
            @Override public void advance(final int offset) { this.row -= offset; }
        }
        public class ReverseHorizontalIterator extends Iterator
        {
            public ReverseHorizontalIterator(final int row, final int column) { super(row, column); }
            public ReverseHorizontalIterator(final Cell cell) { super(cell); }
            public ReverseHorizontalIterator(final Iterator other) { super(other); }
            @Override public void advance(final int offset) { this.column -= offset; }
        }
        public class ReversePrimaryDiagonalIterator extends Iterator
        {
            public ReversePrimaryDiagonalIterator(final int row, final int column) { super(row, column); }
            public ReversePrimaryDiagonalIterator(final Cell cell) { super(cell); }
            public ReversePrimaryDiagonalIterator(final Iterator other) { super(other); }
            @Override public void advance(final int offset) { this.row -= offset; this.column -= offset; }
        }
        public class ReverseSecondaryDiagonalIterator extends Iterator
        {
            public ReverseSecondaryDiagonalIterator(final int row, final int column) { super(row, column); }
            public ReverseSecondaryDiagonalIterator(final Cell cell) { super(cell); }
            public ReverseSecondaryDiagonalIterator(final Iterator other) { super(other); }
            @Override public void advance(final int offset) { this.row -= offset; this.column += offset; }
        }

        /*--------------------------------------------------------*/
        /*--------------------------Move--------------------------*/
        /*--------------------------------------------------------*/
        public class Move
        {
            private Field.Iterator iter;
            public Move(int row, int column) { this.iter = new Field.Iterator(row, column); }
            public Move(Move move) { this.iter = new Field.Iterator(move.get().row(), move.get().column()); }
            public Move(Field.Iterator iter) { this.iter = new Field.Iterator(iter); }

            public Field.Iterator get() { return this.iter; }
            public boolean isValid() { return this.iter.cell().isAvailable(); }
            public void register()
            {
                Collection<Field.Cell> enclosure = this.iter.cell().getEnclosure();
                for (Field.Cell cell : enclosure) cell.changeColor();
                if (Reversi.this.isBlackMove) this.iter.cell().setBlack();
                else this.iter.cell().setWhite();
            }

            @Override public String toString() { return this.iter.cell().toString(); }
        }
    }  
}
