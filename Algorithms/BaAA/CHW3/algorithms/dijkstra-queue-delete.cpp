#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <chrono>
#include <functional>

template <typename Index> // An unsigned integer type
class IndexedPriorityQueue
{
private:
    using Comparator = std::function<bool(Index, Index)>;
    Comparator comparator;
    std::vector<Index> storage;
    std::vector<Index> index;

    void swap(Index first, Index second)
    {
        std::swap(this->index[this->storage[first]], this->index[this->storage[second]]);
        std::swap(this->storage[first], this->storage[second]);
    }
    
    void siftUp(Index index)
    {
        if (index == 0) return;
        const Index parent = (index - 1) >> 1;
        if (this->comparator(this->storage[index], this->storage[parent]))
        {
            this->swap(index, parent);
            this->siftUp(parent);
        }
    }

    void siftDown(Index index)
    {
        const Index left = (index << 1) + 1, right = (index << 1) + 2;
        if (left >= this->storage.size()) return;
        Index to_swap = this->storage.size();
        if (right < this->storage.size())
        {
            // both children exist
            if (this->comparator(this->storage[left], this->storage[index]) || this->comparator(this->storage[right], this->storage[index]))
                to_swap = this->comparator(this->storage[left], this->storage[right]) ? left : right;
        }
        else
        {
            // only left exists
            if (this->comparator(this->storage[left], this->storage[index])) to_swap = left;
        }
        if (to_swap != this->storage.size())
        {
            this->swap(index, to_swap);
            this->siftDown(to_swap);
        }
    }

public:
    IndexedPriorityQueue(Index maxKey, Comparator comparator) : comparator(comparator) { this->index.resize(maxKey, (Index)(-1)); }

    void insert(Index item)
    {
        if (this->index[item] == (Index)(-1))
        {
            this->index[item] = this->storage.size();
            this->storage.push_back(item);
        }
        this->siftUp(this->index[item]);
    }

    Index extract()
    {
        Index to_return = this->storage[0];
        this->index[to_return] = (Index)(-1);
        this->storage[0] = this->storage[this->storage.size() - 1];
        this->index[this->storage[0]] = 0;
        this->storage.pop_back();
        this->siftDown(0);
        return to_return;
    }

    bool empty() { return this->storage.empty(); }
};

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

        IndexedPriorityQueue<VertexIndex> to_update(this->vertexesAmount, [&](VertexIndex a, VertexIndex b) { return distances[a] < distances[b]; });
        to_update.insert(from);
        while (!to_update.empty())
        {
            const VertexIndex from = to_update.extract();
            const EdgeWeight current_distance = distances[from];
            for (const Edge& edge : edges[from])
            {
                const EdgeWeight attempt_distance = current_distance + edge.second;
                if (attempt_distance < distances[edge.first])
                {
                    distances[edge.first] = attempt_distance;
                    to_update.insert(edge.first);
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