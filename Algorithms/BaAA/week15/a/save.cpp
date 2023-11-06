#include <ios>
#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <queue>

struct Edge
{
    unsigned int to;
    uint64_t stream;
    uint64_t flow;
};
std::vector< std::vector<Edge> > edges;
std::vector<unsigned int> distances;
constexpr unsigned int INF = (UINT32_MAX >> 1);

void findDistances(unsigned int from)
{
    distances[from] = 0;
    std::queue<unsigned int> to_update;
    to_update.push(from);
    while (!to_update.empty())
    {
        unsigned int updating = to_update.front();
        to_update.pop();
        for (const Edge& edge : edges[updating])
        {
            if (edge.flow == edge.stream || distances[edge.to] >= distances[updating] + 1) continue;
            distances[edge.to] = distances[updating] + 1;
            to_update.push(edge.to);
        }
    }
}
uint64_t findFlow(unsigned int from, unsigned int to, uint64_t flow = (UINT64_MAX >> 1))
{
    if (from == to || flow == 0) return flow;
}
uint64_t maxFlow(unsigned int from, unsigned int to)
{
    uint64_t answer = 0;
    std::fill(distances.begin(), distances.end(), INF);
    findDistances(from);
    while (distances[to] != INF)
    {
        uint64_t flow = findFlow(from, to);
        while (flow != 0)
        {
            answer += flow;
            flow = findFlow(from, to);
        }
        std::fill(distances.begin(), distances.end(), INF);
        findDistances(from);
    }
    return answer;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n, m;
    std::cin >> n >> m;
    edges.resize(n);
    distances.resize(n);
    for (unsigned int i = 0; i < m; i++)
    {
        unsigned int from, to; uint64_t stream;
        std::cin >> from >> to >> stream;
        edges[from - 1].emplace_back(to - 1, stream);
    }
    
    std::cout << maxFlow(0, n - 1);
}