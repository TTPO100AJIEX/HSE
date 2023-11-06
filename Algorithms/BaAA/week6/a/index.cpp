#include <ios>
#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n, m; std::cin >> n >> m;
    std::vector<std::string> field(n);
    for (unsigned int i = 0; i < n; ++i) std::cin >> field[i];

    uint64_t ans = n * m;
    std::vector<uint64_t> prevDp(m, 1), curDp(m);
    --m;
    curDp[0] = curDp[m] = 1;
    for (unsigned int i = 1; i < n; ++i)
    {
        for (unsigned int j = 1; j < m; ++j)
        {
            char curSymbol = field[i][j];
            if (curSymbol != field[i - 1][j - 1] || curSymbol != field[i - 1][j] || curSymbol != field[i - 1][j + 1])
            {
                curDp[j] = 1;
                continue;
            }
            unsigned int best = std::min(prevDp[j - 1], std::min(prevDp[j], prevDp[j + 1]));
            curDp[j] = best + ((i > best && curSymbol == field[i - best - 1][j]) << 1);
            ans += (curDp[j] >> 1);
        }
        std::swap(prevDp, curDp);
    }

    std::cout << ans;
}
