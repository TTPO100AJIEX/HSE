#include <ios>
#include <iostream>
#include <cstdint>
#include <utility>
#include <vector>

class Graph
{
private:
    std::vector< std::vector <uint16_t> > edges;
    std::vector< std::vector <uint16_t> > edges_transposed;
    std::vector<bool> used;
    std::vector<uint16_t> order;
    std::vector<uint16_t> components;
    uint16_t componentNumber = 0;
    
    void dfsDirect(uint16_t vertex)
    {
        if (this->used[vertex]) return;
        this->used[vertex] = true;
        for (uint16_t sibling : this->edges[vertex]) dfsDirect(sibling);
        this->order.push_back(vertex);
    }
    
    void dfsReversed(uint16_t vertex)
    {
        if (this->components[vertex] != 0) return;
        this->components[vertex] = this->componentNumber;
        for (uint16_t sibling : this->edges_transposed[vertex]) dfsReversed(sibling);
    }

public:
    Graph(uint16_t vertexes)
    {
        this->edges.resize(vertexes);
        this->edges_transposed.resize(vertexes);
    }

    void addEdge(uint16_t from, uint16_t to)
    {
        this->edges[from].push_back(to);
        this->edges_transposed[to].push_back(from);
    }

    std::pair< uint16_t, std::vector<uint16_t> > calculateComponents()
    {
        this->used.resize(this->edges.size(), false);
        this->order.reserve(this->edges.size());
        for (uint16_t i = 0; i < this->edges.size(); ++i) this->dfsDirect(i);

        this->components.resize(this->edges.size());
        for (std::vector<uint16_t>::const_reverse_iterator now = this->order.rbegin(); now != this->order.rend(); ++now)
        {
            if (components[*now] != 0) continue;
            ++this->componentNumber;
            this->dfsReversed(*now);
        }
        return { this->componentNumber, std::move(this->components) };
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
    for (uint16_t component : components) std::cout << component << ' ';
}