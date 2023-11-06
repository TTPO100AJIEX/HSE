#include <ios>
#include <iostream>
#include <cstdint>
#include <vector>

int n, m;
__uint128_t cache[50][30] = { };
__uint128_t solve(int i, int j)
{
    if (i == 0 && j == 0) return 1;
    if (i < 0 || j < 0 || i >= n || j >= m) return 0;
    if (cache[i][j] != 0) return cache[i][j];
    return cache[i][j] = solve(i - 2, j + 1) + solve(i - 2, j - 1) + solve(i + 1, j - 2) + solve(i - 1, j - 2);
}

void print(__uint128_t number)
{
    if (number == 0) return;
    print(number / 10);
    std::cout << (unsigned int)(number % 10);
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::cin >> n >> m;
    __uint128_t ans = solve(n - 1, m - 1);
    if (ans == 0) { std::cout << 0; return 0; }
    print(ans);
}
