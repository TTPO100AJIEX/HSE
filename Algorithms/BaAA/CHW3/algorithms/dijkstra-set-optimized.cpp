#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <chrono>
#include <set>

class Graph
{
public:
    using VertexIndex = uint16_t;
    using EdgeIndex = uint32_t;
    using EdgeWeight = uint64_t;

private:
    static constexpr EdgeWeight WEIGHT_INF = UINT64_MAX / 3;
    using Edge = std::pair<VertexIndex, EdgeWeight>;
    
    VertexIndex vertexesAmount;
    EdgeIndex edgesAmount;
    std::vector< std::vector<Edge> > edges;

public:

    Graph(VertexIndex vertexesAmount, EdgeIndex edgesAmount) : vertexesAmount(vertexesAmount), edgesAmount(edgesAmount)
    {
        this->edges.resize(this->vertexesAmount);
    }

    void addEdge(VertexIndex from, VertexIndex to, EdgeWeight weight)
    {
        this->edges[from].emplace_back(to, weight);
    }

    EdgeWeight distance(VertexIndex from, VertexIndex to)
    {
        std::vector<EdgeWeight> distances(this->vertexesAmount, WEIGHT_INF);
        distances[from] = 0;

        auto cmp = [&](VertexIndex a, VertexIndex b)
        {
            if (distances[a] == distances[b]) return a < b;
            return distances[a] < distances[b];
        };
        std::set<VertexIndex, decltype(cmp)> to_update(cmp);
        to_update.insert(from);
        while (!to_update.empty())
        {
            const VertexIndex from = *(to_update.begin());
            to_update.erase(to_update.begin());
            const EdgeWeight current_distance = distances[from];
            for (const Edge& edge : edges[from])
            {
                const EdgeWeight attempt_distance = current_distance + edge.second;
                if (attempt_distance < distances[edge.first])
                {
                    to_update.erase(edge.first);
                    distances[edge.first] = attempt_distance;
                    to_update.emplace(edge.first);
                }
            }
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