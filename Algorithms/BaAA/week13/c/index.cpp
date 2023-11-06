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
        uint16_t mass;
        bool usedInBest = false;
        bool ignore = false;

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
    Graph(uint16_t vertexes, uint16_t edges)
    {
        this->component_id.resize(vertexes, Edge::NONE);
        this->edges.reserve(edges);
    }

    void addEdge(Edge&& edge)
    {
        this->edges.push_back(std::forward<Edge>(edge));
    }

    uint16_t MSFWeight()
    {
        std::sort(this->edges.begin(), this->edges.end());
        uint16_t answer = 0;
        for (Edge& edge : this->edges)
        {
            uint16_t fromId = this->getComponentId(edge.from);
            uint16_t toId = this->getComponentId(edge.to);
            if (fromId == toId) continue;
            edge.usedInBest = true;
            answer += edge.mass;
            this->component_id[toId] = fromId;
        }
        return answer;
    }
    
    uint16_t SecondMSFWeight()
    {
        uint16_t answer = UINT16_MAX;
        for (Edge& edge : this->edges)
        {
            if (!edge.usedInBest) continue;
            edge.ignore = true;
            
            uint16_t answerAttempt = 0;
            std::fill(this->component_id.begin(), this->component_id.end(), Edge::NONE);
            for (const Edge& edge : this->edges)
            {
                if (edge.ignore) continue;
                uint16_t fromId = this->getComponentId(edge.from);
                uint16_t toId = this->getComponentId(edge.to);
                if (fromId == toId) continue;
                answerAttempt += edge.mass;
                this->component_id[toId] = fromId;
            }
            if (answerAttempt < answer) answer = answerAttempt;

            edge.ignore = false;
        }
        return answer;
    }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    uint16_t n, m;
    std::cin >> n >> m;
    Graph graph(n, m);
    for (uint16_t i = 0; i < m; ++i)
    {
        uint16_t from, to, mass;
        std::cin >> from >> to >> mass;
        --from; --to;
        graph.addEdge({ from, to, mass });
    }

    std::cout << graph.MSFWeight() << ' ' << graph.SecondMSFWeight();
}