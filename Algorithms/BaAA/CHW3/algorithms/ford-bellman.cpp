#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>

class Graph
{
public:
    using VertexIndex = uint16_t;
    using EdgeIndex = uint32_t;
    using EdgeWeight = uint32_t;

private:
    static constexpr EdgeWeight WEIGHT_INF = UINT32_MAX / 3;
    struct Edge
    {
        VertexIndex from;
        VertexIndex to;
        EdgeWeight weight;
    };

    VertexIndex vertexesAmount;
    EdgeIndex edgesAmount;
    std::vector<Edge> edges;

public:

    Graph(VertexIndex vertexesAmount, EdgeIndex edgesAmount) : vertexesAmount(vertexesAmount), edgesAmount(edgesAmount)
    {
        this->edges.reserve(this->edgesAmount);
    }

    void addEdge(VertexIndex from, VertexIndex to, EdgeWeight weight)
    {
        this->edges.emplace_back(from, to, weight);
    }

    EdgeWeight distance(VertexIndex from, VertexIndex to)
    {
        std::vector<EdgeWeight> distances(this->vertexesAmount, WEIGHT_INF);
        distances[from] = 0;
        for (VertexIndex i = 0; i < this->vertexesAmount - 1; ++i)
        {
            bool updated = false;
            for (const Edge& edge : this->edges)
            {
                if (distances[edge.from] == WEIGHT_INF) continue;
                if (distances[edge.to] > distances[edge.from] + edge.weight)
                {
                    distances[edge.to] = distances[edge.from] + edge.weight;
                    updated = true;
                }
            }
            if (!updated) break;
        }
        return distances[to];
    }
};

int main(__attribute__ ((unused)) int argc, char** argv)
{
    std::ifstream fin;
    fin.open(argv[1]);
    
    Graph::VertexIndex vertexes;
    Graph::EdgeIndex edges;
    fin >> vertexes >> edges;

    Graph graph(vertexes, edges);
    for (Graph::EdgeIndex i = 0; i < edges; ++i)
    {
        Graph::EdgeIndex first, second;
        Graph::EdgeWeight weight;
        fin >> first >> second >> weight;
        graph.addEdge(first, second, weight);
        graph.addEdge(second, first, weight);
    }
    
    Graph::VertexIndex from, to;
    fin >> from >> to;
    fin.close();

    using clock = std::conditional_t<std::chrono::high_resolution_clock::is_steady, std::chrono::high_resolution_clock, std::chrono::steady_clock>;
    clock::time_point start = clock::now(); // Get the start timestamp
    const Graph::EdgeWeight answer = graph.distance(from, to);
    clock::time_point end = clock::now(); // Get the end timestamp
    
    std::cout << answer << '\n';
    std::cout << "Time spent: "
                << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << "ns";
}