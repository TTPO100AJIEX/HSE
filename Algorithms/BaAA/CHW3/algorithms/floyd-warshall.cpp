#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <chrono>

class Graph
{
public:
    using VertexIndex = uint16_t;
    using EdgeIndex = uint32_t;
    using EdgeWeight = uint64_t;

private:
    static constexpr EdgeWeight WEIGHT_INF = UINT64_MAX / 3;
    
    VertexIndex vertexesAmount;
    EdgeIndex edgesAmount;
    std::vector< std::vector<EdgeWeight> > edges;

public:

    Graph(VertexIndex vertexesAmount, EdgeIndex edgesAmount) : vertexesAmount(vertexesAmount), edgesAmount(edgesAmount)
    {
        this->edges.resize(this->vertexesAmount, std::vector<EdgeWeight>(this->vertexesAmount, WEIGHT_INF));
        for (VertexIndex i = 0; i < this->vertexesAmount; ++i) this->edges[i][i] = 0;
    }

    void addEdge(VertexIndex from, VertexIndex to, EdgeWeight weight)
    {
        this->edges[from][to] = weight;
    }

    EdgeWeight distance(VertexIndex from, VertexIndex to)
    {
        for (VertexIndex k = 0; k < this->vertexesAmount; ++k)
        {
            std::vector<EdgeWeight>& edgesK = this->edges[k];
            for (VertexIndex i = 0; i < this->vertexesAmount; ++i)
            {
                std::vector<EdgeWeight>& edgesI = this->edges[i];
                EdgeWeight save = edgesI[k];
                for (VertexIndex j = 0; j < this->vertexesAmount; ++j)
                {
                    if (edgesI[j] > save + edgesK[j]) edgesI[j] = save + edgesK[j];
                }
            }
        }
        return this->edges[from][to];
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