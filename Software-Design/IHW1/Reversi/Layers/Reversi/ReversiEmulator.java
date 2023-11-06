package reversi.Layers.Reversi;

import java.util.Collection;

import reversi.utils.Pair;

public class ReversiEmulator extends Reversi
{
    public ReversiEmulator(final Reversi state) { super(null); this.clone(state); }

    public Pair<Double, Field.Move> findBestMove(int moves_to_analyze)
    {
        if (moves_to_analyze == 0) return new Pair<Double, Field.Move>(0.0, null);
        Pair<Double, Field.Move> answer = new Pair<Double, Field.Move>(-1e9, null);
        for (Field.Iterator it = this.field.begin(); it.isValid(); it.next())
        {
            Field.Move move = this.field.new Move(it);
            if (!move.isValid()) continue;

            ReversiEmulator nestedEmulator = this.emulate();
            nestedEmulator.field.new Move(move).register();
            nestedEmulator.isBlackMove = !nestedEmulator.isBlackMove;
            double score = this.calculateRawScore(move) - nestedEmulator.findBestMove(moves_to_analyze - 1).first;

            if (score > answer.first) { answer.first = score; answer.second = move; }
        }
        return answer;
    }
    private double calculateRawScore(Field.Move move)
    {
        Collection<Field.Cell> enclosure = move.get().cell().getEnclosure();
        double score = 0.0;
        if (move.get().cell().isEdge()) score += 0.4; // ss - edge cell
        if (move.get().cell().isCorner()) score += 0.4; // ss - corner cell
        for (Field.Cell cell : enclosure)
        {
            score += 1.0; // si >= 1
            if (cell.isEdge()) score += 1.0; // si = 2
        }
        return score;
    }
}