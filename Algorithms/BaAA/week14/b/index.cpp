#include <ios>
#include <iostream>
#include <cstdint>
#include <vector>
#include <utility>

class Graph
{
private:
    using Edge = std::pair<uint16_t, int>;
    std::vector< std::vector<Edge> > edges;
    std::vector<int64_t> distances;

    void detect_negative_loop(uint16_t vertex)
    {
        if (this->distances[vertex] == NEGINF) return;
        this->distances[vertex] = NEGINF;
        for (const Edge& edge : this->edges[vertex]) this->detect_negative_loop(edge.first);
    }

public:
    constexpr static int64_t INF = (INT64_MAX >> 1);
    constexpr static int64_t NEGINF = (INT64_MIN >> 1);

    Graph(uint16_t vertexes)
    {
        this->edges.resize(vertexes);
        this->distances.resize(vertexes, INF);
    }
    void addEdge(uint16_t from, uint16_t to, int weight)
    {
        this->edges[from].emplace_back(to, weight);
    }

    std::vector<int64_t> findDistances(uint16_t from)
    {
        this->distances[from] = 0;
        for (uint16_t i = 0; i < this->edges.size() - 1; ++i)
        {
            bool updated = false;
            for (uint16_t from = 0; from < this->edges.size(); ++from)
            {
                int64_t cur_distance = this->distances[from];
                if (cur_distance == INF) continue;
                for (const Edge& edge : this->edges[from])
                {
                    int64_t distance_attempt = cur_distance + edge.second;
                    if (distance_attempt < this->distances[edge.first])
                    {
                        this->distances[edge.first] = distance_attempt;
                        updated = true;
                    }
                }
            }
            if (!updated) return std::move(this->distances);
        }

        for (uint16_t from = 0; from < this->edges.size(); ++from)
        {
            int64_t cur_distance = this->distances[from];
            if (cur_distance == INF || cur_distance == NEGINF) continue;
            for (const Edge& edge : this->edges[from])
            {
                int64_t distance_attempt = cur_distance + edge.second;
                if (distance_attempt < this->distances[edge.first])
                {
                    this->detect_negative_loop(from);
                    break;
                }
            }
        }
        return std::move(this->distances);
    }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    uint16_t n, m;
    std::cin >> n >> m;
    Graph graph(n);
    for (uint16_t i = 0; i < m; ++i)
    {
        uint16_t from, to;
        int weight;
        std::cin >> from >> to >> weight;
        graph.addEdge(from, to, weight);
    }

    std::vector<int64_t> distances = graph.findDistances(0);
    for (uint16_t i = 1; i < n; ++i)
    {
        if (distances[i] == Graph::NEGINF) std::cout << "-inf\n";
        else std::cout << distances[i] << '\n';
    }
}