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

void addEdge(unsigned int from, unsigned int to, unsigned int max_flow)
{
    edges[from].emplace_back(to, edges[to].size(), max_flow);
    edges[to].emplace_back(from, edges[from].size() - 1, 0);
}

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
    unsigned int n, m;
    std::cin >> n >> m;
    edges.resize(n * m + 2);
    distances.resize(edges.size());

    unsigned int sum_connections = 0;
    for (unsigned int row = 0; row < n; ++row)
    {
        for (unsigned int col = 0; col < m; ++col)
        {
            char symbol;
            std::cin >> symbol;
            unsigned int connections;
            switch (symbol)
            {
                case 'H': { connections = 1; break; }
                case 'O': { connections = 2; break; }
                case 'N': { connections = 3; break; }
                case 'C': { connections = 4; break; }
                default: { connections = 0; }
            }
            sum_connections += connections;
            if (col % 2 == row % 2)
            {
                addEdge(0, row * m + col + 1, connections);
                if (row != 0) addEdge(row * m + col + 1, (row - 1) * m + col + 1, 1);
                if (row != n - 1) addEdge(row * m + col + 1, (row + 1) * m + col + 1, 1);
                if (col != 0) addEdge(row * m + col + 1, row * m + (col - 1) + 1, 1);
                if (col != m - 1) addEdge(row * m + col + 1, row * m + (col + 1) + 1, 1);
            }
            else
            {
                addEdge(row * m + col + 1, n * m + 1, connections);
            }
        }
    }
    
    unsigned int flow = maxFlow(0, n * m + 1);
    std::cout << ((flow << 1) == sum_connections ? "Valid" : "Invalid");
}