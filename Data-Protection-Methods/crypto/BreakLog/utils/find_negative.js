function gcd(a, b)
{
    if (b == 0) return a;
    return(gcd(b, a % b));
}
function factor(a, b) // a > b, gcd(a, b) = 1
{
    let q = Math.floor(a / b), r = a % b;
    if (r == 1) return([ 1, -q ]);
    let [ k1, k2 ] = factor(b, r);
    return([ k2, k1 - k2 * q ]);
}


export default function find_negative(n, mod)
{
    if (gcd(mod, n) != 1) return -1;
    let ans = factor(mod, n)[1];
    ans %= mod;
    return(ans < 0 ? ans + mod : ans);
}