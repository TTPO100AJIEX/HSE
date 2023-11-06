#include <ios>
#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <utility>
#include <algorithm>

class Graph {
private:
    struct Edge {
        static constexpr uint16_t kNone = UINT16_MAX;

        uint16_t from;
        uint16_t to;
        unsigned int mass;

        bool operator<(const Edge& other) const {
            return this->mass < other.mass;
        }
    };
    std::vector<Edge> edges_;
    std::vector<uint16_t> component_id_;

    uint16_t getComponentId(uint16_t vertex) {
        if (this->component_id_[vertex] == Edge::kNone) {
            return vertex;
        }
        return this->component_id_[vertex] = this->getComponentId(this->component_id_[vertex]);
    }

public:
    Graph(uint16_t vertexes, unsigned int edges) {
        this->component_id_.resize(vertexes, Edge::kNone);
        this->edges_.reserve(edges);
    }

    void addEdge(Edge&& edge) {
        this->edges_.push_back(std::forward<Edge>(edge));
    }

    uint64_t msfWeight() {
        std::sort(this->edges_.begin(), this->edges_.end());
        uint64_t answer = 0;
        for (const Edge& edge : this->edges_) {
            uint16_t from_id = this->getComponentId(edge.from);
            uint16_t to_id = this->getComponentId(edge.to);
            if (from_id == to_id) {
                continue;
            }
            answer += edge.mass;
            if (std::rand() & 1) {
                this->component_id_[to_id] = from_id;
            } else {
                this->component_id_[from_id] = to_id;
            }
        }
        return answer;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n, m;
    std::cin >> n >> m;
    Graph graph(n, m);
    for (unsigned int i = 0; i < m; ++i) {
        uint16_t from, to;
        unsigned int mass;
        std::cin >> from >> to >> mass;
        --from;
        --to;
        graph.addEdge({from, to, mass});
    }

    std::cout << graph.msfWeight();
}