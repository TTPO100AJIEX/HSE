package ru.hse;

public class Func
{
    public int gcd(int a, int b)
    {
        if (a == 0) return b;
        if (b == 0) return a;
        if (a > 0 && b < 0 || a < 0 && b > 0) b = -b;

        do
        {
            if (a == b) return a;
            if (b > a && a > 0 || b < a && a < 0)
            {
                a = b - a;
                b = b - a;
                a = a + b;
            }

            b = a - b;
            a = a - b;
        } while (b != 0);

        return a;
    }
}