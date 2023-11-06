#include <ios>
#include <iostream>
#include <climits>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <string>

struct Edge
{
    unsigned int to;
    unsigned int reverse_edge;
    int cur_flow = 0;

    Edge(unsigned int to, unsigned int reverse_edge) : to(to), reverse_edge(reverse_edge) { }
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
            if (edge.cur_flow >= 1 || distances[edge.to] <= distances[updating] + 1) continue;
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
        unsigned int add_flow = findFlow(edge.to, to, std::min(flow, (unsigned int)(1 - edge.cur_flow)));
        if (add_flow == 0) continue;
        edge.cur_flow += add_flow;
        edges[edge.to][edge.reverse_edge].cur_flow -= add_flow;
        return add_flow;
    }
    return 0;
}
unsigned int maxFlow(unsigned int from, unsigned int to)
{
    unsigned int answer = 0;
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

    unsigned int n, m, k;
    std::cin >> n >> m >> k;
    std::unordered_map<std::string, unsigned int> vertexes;
    vertexes.reserve(n);
    edges.resize(n);
    distances.resize(n);

    std::string first, second;
    for (unsigned int i = 0; i < m; ++i)
    {
        std::cin >> first >> second;
        unsigned int firstIndex = vertexes.emplace(first, vertexes.size()).first->second;
        unsigned int secondIndex = vertexes.emplace(second, vertexes.size()).first->second;
        edges[firstIndex].emplace_back(secondIndex, edges[secondIndex].size());
        edges[secondIndex].emplace_back(firstIndex, edges[firstIndex].size() - 1);
    }

    for (unsigned int test = 0; test < k; ++test)
    {
        for (unsigned int from = 0; from < n; ++from)
        {
            for (unsigned int i = 0; i < edges[from].size(); ++i) edges[from][i].cur_flow = 0;
        }
        std::cin >> first >> second;
        unsigned int firstIndex = vertexes.emplace(first, vertexes.size()).first->second;
        unsigned int secondIndex = vertexes.emplace(second, vertexes.size()).first->second;
        if (firstIndex == secondIndex) std::cout << 0 << '\n';
        else std::cout << maxFlow(firstIndex, secondIndex) << '\n';
    }
}