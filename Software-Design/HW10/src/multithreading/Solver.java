package multithreading;

import java.math.BigInteger;
import java.util.ArrayList;
import java.util.List;

public class Solver extends Thread
{
    private BigInteger answer;
    public Solver(List<String> data, int numThreads) throws InterruptedException
    {
        ArrayList<Solver> solvers = new ArrayList<Solver>();
        for (int i = 0; i < numThreads; i++)
        {
            solvers.add(new Solver(data, i * data.size() / numThreads, (i + 1) * data.size() / numThreads));
            solvers.get(solvers.size() - 1).start();
        }
        this.answer = BigInteger.ZERO;
        for (Solver solver : solvers)
        {
            solver.join();
            this.answer = this.answer.add(solver.getAnswer());
        }
    }
    
    private int start, end;
    private List<String> data;
    public Solver(List<String> data, int start, int end) { this.data = data; this.start = start; this.end = end; }

    @Override
    public void run()
    {
        this.answer = BigInteger.ZERO;
        for (int i = this.start; i < this.end; i++) this.answer = this.answer.add(BigInteger.valueOf(this.data.get(i).length()));
    }

    public BigInteger getAnswer() { return this.answer; }
};
