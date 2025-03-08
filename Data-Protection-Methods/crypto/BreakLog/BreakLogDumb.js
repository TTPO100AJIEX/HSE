export default function BreakLogDumb(q, b, y)
{
    let last_power = 1, x = 0;
    do {
        if (last_power == y) return x;
        x++; last_power = (last_power * b) % q;
    } while (last_power != 1);
    return -1;
};