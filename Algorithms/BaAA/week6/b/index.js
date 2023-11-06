/*var s = "std::string answers[10][30] = {\n";
for (let i = 40; i < 50; i++)
{
    s += "    { "
    for (let j = 0; j < 30; j++)
    {
        let cache = { };
        function f(u, v)
        {
            if (u == 0 && v == 0) return 1n;
            if (u < 0 || v < 0 || u > i || v > j) return 0n;
            cache[u] ??= [ ];
            if (cache[u][v]) return cache[u][v];
            return cache[u][v] = f(u - 2, v + 1) + f(u - 2, v - 1) + f(u + 1, v - 2) + f(u - 1, v - 2);
        }
        s += `"${f(i, j)}", `;
        if (s.substring(s.lastIndexOf("\n") + 1).length > 50) s += "\n     ";
    }
    s = s.slice(0, -2) + " },\n";
}
s += "};"
const fs = require('fs');
fs.writeFileSync("res.cpp", s, "utf-8");
*/

let max = 0n;
for (let i = 0; i < 50; i++)
{
    for (let j = 0; j < 30; j++)
    {
        let cache = { };
        function f(u, v)
        {
            if (u == 0 && v == 0) return 1n;
            if (u < 0 || v < 0 || u > i || v > j) return 0n;
            cache[u] ??= [ ];
            if (cache[u][v]) return cache[u][v];
            return cache[u][v] = f(u - 2, v + 1) + f(u - 2, v - 1) + f(u + 1, v - 2) + f(u - 1, v - 2);
        }
        let ans = f(i, j);
        if (ans > max) max = ans;
    }
}
console.log(max);