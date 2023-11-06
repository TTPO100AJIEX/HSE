#include <ios>
#include <iostream>
#include <climits>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <queue>

struct Edge
{
    unsigned int to;
    unsigned int reverse_edge;
    int cur_flow = 0;
    unsigned int max_flow;

    Edge(unsigned int to, unsigned int reverse_edge, int max_flow) : to(to), reverse_edge(reverse_edge), max_flow(max_flow) { }
};
std::vector< std::vector<Edge> > edges;
std::vector<unsigned int> distances;
constexpr unsigned int INF = (UINT_MAX >> 1);

void findDistances(unsigned int from)
{
    distances[from] = 0;
    static std::queue<unsigned int> to_update;
    to_update.push(from);
    while (!to_update.empty())
    {
        unsigned int updating = to_update.front();
        to_update.pop();
        for (const Edge& edge : edges[updating])
        {
            if (edge.cur_flow >= (int)(edge.max_flow) || distances[edge.to] <= distances[updating] + 1) continue;
            distances[edge.to] = distances[updating] + 1;
            to_update.push(edge.to);
        }
    }
}
unsigned int findFlow(unsigned int from, unsigned int to, unsigned int flow = (UINT_MAX >> 1))
{
    if (from == to || flow == 0) return flow;
    for (Edge& edge : edges[from])
    {
        if (distances[edge.to] != distances[from] + 1) continue;
        unsigned int add_flow = findFlow(edge.to, to, std::min(flow, (unsigned int)((int)(edge.max_flow) - edge.cur_flow)));
        if (add_flow == 0) continue;
        edge.cur_flow += add_flow;
        edges[edge.to][edge.reverse_edge].cur_flow -= add_flow;
        return add_flow;
    }
    return 0;
}
uint64_t maxFlow(unsigned int from, unsigned int to)
{
    uint64_t answer = 0;
    std::fill(distances.begin(), distances.end(), INF);
    findDistances(from);
    while (distances[to] != INF)
    {
        unsigned int flow = findFlow(from, to);
        while (flow != 0) { answer += flow; flow = findFlow(from, to); }
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
    distances.resize(edges.size());
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int from, to, max_flow;
        std::cin >> from >> to >> max_flow;
        --from; --to;
        edges[from].emplace_back(to, edges[to].size(), max_flow);
        edges[to].emplace_back(from, edges[from].size() - 1, 0);
    }
    
    std::cout << maxFlow(0, n - 1);
}