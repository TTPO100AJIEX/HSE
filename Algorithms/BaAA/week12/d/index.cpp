#include <ios>
#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <algorithm>


class Graph
{
private:
    struct Vertex
    {
        std::vector<uint16_t> edges;
        bool isSrc = true;
        bool used = false;
    };
    std::vector<Vertex> vertexes;
    uint16_t srcesCounter;

    void dfs(uint16_t vertexIndex, std::vector<uint16_t>& order)
    {
        Vertex& vertex = this->vertexes[vertexIndex];
        if (vertex.used) return;
        vertex.used = true;
        for (uint16_t sibling : vertex.edges) this->dfs(sibling, order);
        order.push_back(vertexIndex);
    }

public:
    Graph(uint16_t vertexes)
    {
        this->vertexes.resize(vertexes);
        this->srcesCounter = vertexes;
    }

    void addEdge(uint16_t from, uint16_t to)
    {
        this->vertexes[from].edges.push_back(to);
        if (this->vertexes[to].isSrc)
        {
            --this->srcesCounter;
            this->vertexes[to].isSrc = false;
        }
    }

    template<typename sortFunction>
    std::vector<uint16_t> topSort(sortFunction sorter)
    {
        std::vector<uint16_t> srces;
        srces.reserve(this->srcesCounter);
        for (unsigned int i = 0; i < this->vertexes.size(); ++i)
        {
            if (this->vertexes[i].isSrc) srces.push_back(i);
        }

        std::sort(srces.begin(), srces.end(), sorter);
        for (Vertex& vertex : vertexes) std::sort(vertex.edges.begin(), vertex.edges.end(), sorter);

        std::vector<uint16_t> order;
        order.reserve(this->vertexes.size());
        for (uint16_t src : srces) this->dfs(src, order);
        return order;
    }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n, m;
    std::cin >> n >> m;
    
    std::vector<std::string> names(n);
    for (uint16_t i = 0; i < n; ++i)
    {
        uint16_t index; std::cin >> index;
        std::cin.ignore(1); std::getline(std::cin, names[--index]);
    }

    Graph graph(n);
    for (unsigned int i = 0; i < m; ++i)
    {
        uint16_t from, to;
        std::cin >> from >> to;
        graph.addEdge(from - 1, to - 1);
    }

    std::vector<uint16_t> order = graph.topSort([&names](const uint16_t first, const uint16_t second) { return names[first] < names[second]; });
    for (std::vector<uint16_t>::const_reverse_iterator now = order.rbegin(); now != order.rend(); ++now) std::cout << names[*now] << '\n';
}