#include <ios>
#include <iostream>
#include <cstdint>
#include <queue>
#include <vector>
#include <utility>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    unsigned int n, m;
    std::cin >> n >> m;

    using Edge = std::pair<unsigned int, unsigned int>;
    std::vector< std::vector<Edge> > edges(n);
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int from, to, weight;
        std::cin >> from >> to >> weight;
        edges[from].emplace_back(to, weight);
    }

    constexpr uint64_t INF = UINT64_MAX;
    std::vector<uint64_t> distances(n, INF);
    distances[0] = 0;

    using Result = std::pair<uint64_t, unsigned int>;
    std::priority_queue<Result, std::vector<Result>, std::greater<Result>> to_update;
    to_update.emplace(0, 0);
    while (!to_update.empty())
    {
        const auto [ current_distance, from ] = to_update.top();
        to_update.pop();
        if (current_distance != distances[from]) continue;
        for (const Edge& edge : edges[from])
        {
            uint64_t attempt_distance = current_distance + edge.second;
            if (attempt_distance < distances[edge.first])
            {
                distances[edge.first] = attempt_distance;
                to_update.emplace(attempt_distance, edge.first);
            }
        }
    }

    for (unsigned int i = 1; i < n; ++i) std::cout << distances[i] << '\n';
}