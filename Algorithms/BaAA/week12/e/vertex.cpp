#include <ios>
#include <iostream>
#include <cstdint>
#include <utility>
#include <vector>

class Graph
{
private:
    struct Vertex
    {
        std::vector<uint16_t> edges;
        std::vector<uint16_t> edges_transposed;
        bool used = false;
        uint16_t component = 0;
    };
    std::vector<Vertex> vertexes;

    void dfsDirect(uint16_t vertexIndex, std::vector<uint16_t>& order)
    {
        Vertex& vertex = this->vertexes[vertexIndex];
        if (vertex.used) return;
        vertex.used = true;
        for (uint16_t sibling : vertex.edges) this->dfsDirect(sibling, order);
        order.push_back(vertexIndex);
    }

    void dfsReversed(uint16_t vertexIndex, uint16_t component)
    {
        Vertex& vertex = this->vertexes[vertexIndex];
        if (vertex.component != 0) return;
        vertex.component = component;
        for (uint16_t sibling : vertex.edges_transposed) this->dfsReversed(sibling, component);
    }

public:
    Graph(uint16_t vertexes)
    {
        this->vertexes.resize(vertexes);
    }

    void addEdge(uint16_t from, uint16_t to)
    {
        this->vertexes[from].edges.push_back(to);
        this->vertexes[to].edges_transposed.push_back(from);
    }

    std::pair< uint16_t, std::vector<Vertex> > calculateComponents()
    {
        std::vector<uint16_t> order;
        order.reserve(this->vertexes.size());
        for (uint16_t i = 0; i < this->vertexes.size(); ++i) this->dfsDirect(i, order);
        
        uint16_t componentNumber = 0;
        for (std::vector<uint16_t>::const_reverse_iterator now = order.rbegin(); now != order.rend(); ++now)
        {
            if (this->vertexes[*now].component != 0) continue;
            this->dfsReversed(*now, ++componentNumber);
        }
        return { componentNumber, std::move(this->vertexes) };
    }
};


int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n, m;
    std::cin >> n >> m;
    Graph graph(n);
    for (unsigned int i = 0; i < m; ++i)
    {
        uint16_t from, to;
        std::cin >> from >> to;
        graph.addEdge(from - 1, to - 1);
    }
    const auto [ componentsAmount, components ] = graph.calculateComponents();

    std::cout << componentsAmount << '\n';
    for (const auto& component : components) std::cout << component.component << ' ';
}