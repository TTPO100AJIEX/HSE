#include <ios>
#include <iostream>
#include <cstdint>
#include <vector>
#include <utility>
#include <algorithm>

class Graph
{
private:
    struct Edge
    {
        static constexpr uint16_t NONE = UINT16_MAX;

        uint16_t from;
        uint16_t to;
        unsigned int mass;

        bool operator<(const Edge& other) const { return this->mass < other.mass; }
    };
    std::vector<Edge> edges;
    std::vector<uint16_t> component_id;

    uint16_t getComponentId(uint16_t vertex)
    {
        if (this->component_id[vertex] == Edge::NONE) return vertex;
        return this->component_id[vertex] = this->getComponentId(this->component_id[vertex]);
    }

public:
    Graph(uint16_t vertexes, unsigned int edges)
    {
        this->component_id.resize(vertexes, Edge::NONE);
        this->edges.reserve(edges);
    }

    void addEdge(Edge&& edge)
    {
        this->edges.push_back(std::forward<Edge>(edge));
    }

    uint64_t MSFWeight()
    {
        std::sort(this->edges.begin(), this->edges.end());
        uint64_t answer = 0;
        for (const Edge& edge : this->edges)
        {
            uint16_t fromId = this->getComponentId(edge.from);
            uint16_t toId = this->getComponentId(edge.to);
            if (fromId == toId) continue;
            answer += edge.mass;
            this->component_id[toId] = fromId;
        }
        return answer;
    }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n, m;
    std::cin >> n >> m;
    Graph graph(n, m);
    for (unsigned int i = 0; i < m; ++i)
    {
        uint16_t from, to;
        unsigned int mass;
        std::cin >> from >> to >> mass;
        --from; --to;
        graph.addEdge({ from, to, mass });
    }

    std::cout << graph.MSFWeight();
}