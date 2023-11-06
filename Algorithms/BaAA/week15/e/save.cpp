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
    int cur_flow;
    int max_flow;
};
std::vector< std::vector<Edge> > edges;
std::vector<unsigned int> distances;
constexpr unsigned int INF = (UINT_MAX >> 1);

void findDistances(unsigned int from)
{
    distances[from] = 0;
    std::queue<unsigned int> to_update;
    to_update.push(from);
    while (!to_update.empty())
    {
        unsigned int updating = to_update.front();
        to_update.pop();
        for (unsigned int to = 0; to < edges.size(); to++)
        {
            if (edges[updating][to].cur_flow >= edges[updating][to].max_flow || distances[to] <= distances[updating] + 1) continue;
            distances[to] = distances[updating] + 1;
            to_update.push(to);
        }
    }
}
int findFlow(unsigned int from, unsigned int to, int flow = (INT_MAX >> 1))
{
    if (from == to || flow == 0) return flow;
    for (unsigned int i = 0; i < edges.size(); i++)
    {
        if (distances[i] != distances[from] + 1) continue;
        int add_flow = findFlow(i, to, std::min(flow, edges[from][i].max_flow - edges[from][i].cur_flow));
        if (add_flow != 0)
        {
            edges[from][i].cur_flow += add_flow;
            edges[i][from].cur_flow -= add_flow;
            return add_flow;
        }
    }
    return 0;
}
int maxFlow(unsigned int from, unsigned int to)
{
    int answer = 0;
    distances.resize(edges.size());
    std::fill(distances.begin(), distances.end(), INF);
    findDistances(from);
    while (distances[to] != INF)
    {
        int flow = findFlow(from, to);
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

    unsigned int n, m, k;
    std::cin >> n >> m >> k;
    std::unordered_map<std::string, unsigned int> vertexes;
    vertexes.reserve(n);
    edges.resize(n, std::vector<Edge>(n, { 0, 0 }));
    std::string first, second;
    for (unsigned int i = 0; i < m; i++)
    {
        std::cin >> first >> second;
        unsigned int firstIndex = vertexes.emplace(first, vertexes.size()).first->second;
        unsigned int secondIndex = vertexes.emplace(second, vertexes.size()).first->second;
        edges[firstIndex][secondIndex].max_flow++;
        edges[secondIndex][firstIndex].max_flow++;
    }

    for (unsigned int i = 0; i < k; i++)
    {
        for (unsigned int i1 = 0; i1 < n; i1++)
        {
            for (unsigned int i2 = 0; i2 < n; i2++)
            {
                edges[i1][i2].cur_flow = 0;
            }
        }
        std::cin >> first >> second;
        unsigned int firstIndex = vertexes.emplace(first, vertexes.size()).first->second;
        unsigned int secondIndex = vertexes.emplace(second, vertexes.size()).first->second;
        if (firstIndex == secondIndex) std::cout << 0 << '\n';
        else std::cout << maxFlow(firstIndex, secondIndex) << '\n';
    }
}