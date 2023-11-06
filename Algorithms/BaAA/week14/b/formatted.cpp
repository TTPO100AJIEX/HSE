#include <ios>
#include <iostream>
#include <cstdint>
#include <vector>
#include <utility>

class Graph {
private:
    using Edge = std::pair<uint16_t, int>;
    std::vector<std::vector<Edge> > edges_;
    std::vector<int64_t> distances_;

    void detectNegativeLoop(uint16_t vertex) {
        if (this->distances_[vertex] == kNeginf) {
            return;
        }
        this->distances_[vertex] = kNeginf;
        for (const Edge& edge : this->edges_[vertex]) {
            this->detectNegativeLoop(edge.first);
        }
    }

public:
    constexpr static int64_t kInf = (INT64_MAX >> 1);
    constexpr static int64_t kNeginf = (INT64_MIN >> 1);

    Graph(uint16_t vertexes) {
        this->edges_.resize(vertexes);
        this->distances_.resize(vertexes, kInf);
    }
    void addEdge(uint16_t from, uint16_t to, int weight) {
        this->edges_[from].emplace_back(to, weight);
    }

    std::vector<int64_t> findDistances(uint16_t from) {
        this->distances_[from] = 0;
        for (uint16_t i = 0; i < this->edges_.size() - 1; ++i) {
            bool updated = false;
            for (uint16_t from = 0; from < this->edges_.size(); ++from) {
                int64_t cur_distance = this->distances_[from];
                if (cur_distance == kInf) {
                    continue;
                }
                for (const Edge& edge : this->edges_[from]) {
                    int64_t distance_attempt = cur_distance + edge.second;
                    if (distance_attempt < this->distances_[edge.first]) {
                        this->distances_[edge.first] = distance_attempt;
                        updated = true;
                    }
                }
            }
            if (!updated) {
                return std::move(this->distances_);
            }
        }

        for (uint16_t from = 0; from < this->edges_.size(); ++from) {
            int64_t cur_distance = this->distances_[from];
            if (cur_distance == kInf || cur_distance == kNeginf) {
                continue;
            }
            for (const Edge& edge : this->edges_[from]) {
                int64_t distance_attempt = cur_distance + edge.second;
                if (distance_attempt < this->distances_[edge.first]) {
                    this->detectNegativeLoop(from);
                    break;
                }
            }
        }
        return std::move(this->distances_);
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    uint16_t n, m;
    std::cin >> n >> m;
    Graph graph(n);
    for (uint16_t i = 0; i < m; ++i) {
        uint16_t from, to;
        int weight;
        std::cin >> from >> to >> weight;
        graph.addEdge(from, to, weight);
    }

    std::vector<int64_t> distances = graph.findDistances(0);
    for (uint16_t i = 1; i < n; ++i) {
        if (distances[i] == Graph::kNeginf) {
            std::cout << "-inf\n";
        } else {
            std::cout << distances[i] << '\n';
        }
    }
}