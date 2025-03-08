var cache = new Map();
export default function pow(a, b, mod) // a ^ b % mod
{
    return Number(pow_bigint(BigInt(a), BigInt(b), BigInt(mod)));
}

function pow_bigint(a, b, mod) // a ^ b % mod
{
    if (b == 0n) return 1n;
    if (cache.has(`${a}^${b}`)) return cache.get(`${a}^${b}`);

    let res = pow_bigint(a, b >> 1n, mod);
    res = (res * res) % mod;
    if (b & 1n) res = (a * res) % mod;

    cache.set(`${a}^${b}`, res);
    return res;
}

function clear_cache() { cache.clear(); }

export { clear_cache };