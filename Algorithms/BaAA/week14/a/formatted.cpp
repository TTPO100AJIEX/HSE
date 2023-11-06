#include <ios>
#include <iostream>
#include <cstdint>
#include <climits>
#include <queue>
#include <vector>
#include <utility>
#include <functional>

class PriorityQueue {
private:
    using Comparator = std::function<bool(unsigned int, unsigned int)>;
    Comparator comparator_;
    std::vector<unsigned int> storage_;
    std::vector<unsigned int> index_;

    void swap(unsigned int first, unsigned int second) {
        std::swap(this->index_[this->storage_[first]], this->index_[this->storage_[second]]);
        std::swap(this->storage_[first], this->storage_[second]);
    }

    void siftUp(unsigned int index) {
        if (index == 0) {
            return;
        }
        const unsigned int parent = (index - 1) >> 1;
        if (this->comparator_(this->storage_[index], this->storage_[parent])) {
            this->swap(index, parent);
            this->siftUp(parent);
        }
    }

    void siftDown(unsigned int index) {
        const unsigned int left = (index << 1) + 1, right = (index << 1) + 2;
        if (left >= this->storage_.size()) {
            return;
        }
        unsigned int to_swap = this->storage_.size();
        if (right < this->storage_.size()) {
            // both children exist
            if (this->comparator_(this->storage_[left], this->storage_[index]) ||
                this->comparator_(this->storage_[right], this->storage_[index])) {
                to_swap =
                    this->comparator_(this->storage_[left], this->storage_[right]) ? left : right;
            }
        } else {
            // only left exists
            if (this->comparator_(this->storage_[left], this->storage_[index])) {
                to_swap = left;
            }
        }
        if (to_swap != this->storage_.size()) {
            this->swap(index, to_swap);
            this->siftDown(to_swap);
        }
    }

public:
    PriorityQueue(unsigned int max_key, Comparator comparator) : comparator_(comparator) {
        this->index_.resize(max_key, UINT_MAX);
    }

    void insert(unsigned int item) {
        if (this->index_[item] == UINT_MAX) {
            this->index_[item] = this->storage_.size();
            this->storage_.push_back(item);
        }
        this->siftUp(this->index_[item]);
    }

    unsigned int extract() {
        unsigned int to_return = this->storage_[0];
        this->index_[to_return] = UINT_MAX;
        this->storage_[0] = this->storage_[this->storage_.size() - 1];
        this->index_[this->storage_[0]] = 0;
        this->storage_.pop_back();
        this->siftDown(0);
        return to_return;
    }

    bool empty() {
        return this->storage_.empty();
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n, m;
    std::cin >> n >> m;

    using Edge = std::pair<unsigned int, unsigned int>;
    std::vector<std::vector<Edge> > edges(n);
    for (unsigned int i = 0; i < m; ++i) {
        unsigned int from, to, weight;
        std::cin >> from >> to >> weight;
        edges[from].emplace_back(to, weight);
    }

    constexpr uint64_t kInf = UINT64_MAX;
    std::vector<uint64_t> distances(n, kInf);
    distances[0] = 0;

    PriorityQueue to_update(n, [&](unsigned int a, unsigned int b) {
        return distances[a] == distances[b] ? a < b : distances[a] < distances[b];
    });
    to_update.insert(0);
    while (!to_update.empty()) {
        unsigned int from = to_update.extract();
        for (const Edge& edge : edges[from]) {
            uint64_t attempt_distance = distances[from] + edge.second;
            if (attempt_distance < distances[edge.first]) {
                distances[edge.first] = attempt_distance;
                to_update.insert(edge.first);
            }
        }
    }

    for (unsigned int i = 1; i < n; ++i) {
        std::cout << distances[i] << '\n';
    }
}