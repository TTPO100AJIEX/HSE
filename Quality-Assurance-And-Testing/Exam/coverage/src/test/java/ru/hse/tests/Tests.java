package ru.hse.tests;

import ru.hse.*;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

public class Tests
{
    @Test
    public void testFunc()
    {
        Func a = new Func();
        a.gcd(1, -2);
    }
}
