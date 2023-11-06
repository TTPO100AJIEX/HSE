#include <ios>
#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    uint16_t n;
    unsigned int m;
    std::cin >> n >> m;

    constexpr uint64_t INF = UINT64_MAX / 2;
    std::vector< std::vector<uint64_t> > edges(n, std::vector<uint64_t>(n, INF));

    for (unsigned int i = 0; i < m; ++i)
    {
        uint16_t from, to;
        unsigned int weight;
        std::cin >> from >> to >> weight;
        edges[from][to] = weight;
    }
    for (uint16_t i = 0; i < n; ++i) edges[i][i] = 0;

    for (uint16_t k = 0; k < n; ++k)
    {
        for (uint16_t i = 0; i < n; ++i)
        {
            uint64_t save = edges[i][k];
            for (uint16_t j = 0; j < n; ++j)
            {
                edges[i][j] = std::min(edges[i][j], save + edges[k][j]);
            }
        }
    }

    for (uint16_t i = 0; i < n; ++i)
    {
        for (uint16_t j = 0; j < n; ++j)
        {
            if (i == j) continue;
            std::cout << i << ' ' << j << ' ' << static_cast<int64_t>(edges[i][j] == INF ? -1 : edges[i][j]) << '\n';
        }
    }
}