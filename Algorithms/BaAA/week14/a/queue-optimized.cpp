#include <ios>
#include <iostream>
#include <cstdint>
#include <climits>
#include <queue>
#include <vector>
#include <utility>
#include <functional>

class PriorityQueue
{
private:
    using Comparator = std::function<bool(unsigned int, unsigned int)>;
    Comparator comparator;
    std::vector<unsigned int> storage;
    std::vector<unsigned int> index;

    void swap(unsigned int first, unsigned int second)
    {
        std::swap(this->index[this->storage[first]], this->index[this->storage[second]]);
        std::swap(this->storage[first], this->storage[second]);
    }
    
    void siftUp(unsigned int index)
    {
        if (index == 0) return;
        const unsigned int parent = (index - 1) >> 1;
        if (this->comparator(this->storage[index], this->storage[parent]))
        {
            this->swap(index, parent);
            this->siftUp(parent);
        }
    }

    void siftDown(unsigned int index)
    {
        const unsigned int left = (index << 1) + 1, right = (index << 1) + 2;
        if (left >= this->storage.size()) return;
        unsigned int to_swap = this->storage.size();
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
    PriorityQueue(unsigned int maxKey, Comparator comparator) : comparator(comparator) { this->index.resize(maxKey, UINT_MAX); }

    void insert(unsigned int item)
    {
        if (this->index[item] == UINT_MAX)
        {
            this->index[item] = this->storage.size();
            this->storage.push_back(item);
        }
        this->siftUp(this->index[item]);
    }

    unsigned int extract()
    {
        unsigned int to_return = this->storage[0];
        this->index[to_return] = UINT_MAX;
        this->storage[0] = this->storage[this->storage.size() - 1];
        this->index[this->storage[0]] = 0;
        this->storage.pop_back();
        this->siftDown(0);
        return to_return;
    }

    bool empty() { return this->storage.empty(); }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    unsigned int n, m;
    std::cin >> n >> m;

    using Edge = std::pair<unsigned int, unsigned int>;
    std::vector< std::vector<Edge> > edges(n);
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int from, to, weight;
        std::cin >> from >> to >> weight;
        edges[from].emplace_back(to, weight);
    }

    constexpr uint64_t INF = UINT64_MAX;
    std::vector<uint64_t> distances(n, INF);
    distances[0] = 0;

    PriorityQueue to_update(n, [&](unsigned int a, unsigned int b) { return distances[a] < distances[b]; });
    to_update.insert(0);
    while (!to_update.empty())
    {
        unsigned int from = to_update.extract();
        for (const Edge& edge : edges[from])
        {
            uint64_t attempt_distance = distances[from] + edge.second;
            if (attempt_distance < distances[edge.first])
            {
                distances[edge.first] = attempt_distance;
                to_update.insert(edge.first);
            }
        }
    }
    
    for (unsigned int i = 1; i < n; ++i) std::cout << distances[i] << '\n';
}