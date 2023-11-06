#include <ios>
#include <iostream>
#include <cstdint>
#include <vector>
#include <utility>
#include <algorithm>

class Graph {
private:
    struct Edge {
        static constexpr uint16_t kNone = UINT16_MAX;

        uint16_t from;
        uint16_t to;
        uint16_t mass;
        bool usedInBest = false;
        bool ignore = false;

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
    Graph(uint16_t vertexes, uint16_t edges) {
        this->component_id_.resize(vertexes, Edge::kNone);
        this->edges_.reserve(edges);
    }

    void addEdge(Edge&& edge) {
        this->edges_.push_back(std::forward<Edge>(edge));
    }

    uint32_t msfWeight() {
        std::sort(this->edges_.begin(), this->edges_.end());
        uint32_t answer = 0;
        for (Edge& edge : this->edges_) {
            uint16_t from_id = this->getComponentId(edge.from);
            uint16_t to_id = this->getComponentId(edge.to);
            if (from_id == to_id) {
                continue;
            }
            edge.usedInBest = true;
            answer += edge.mass;
            this->component_id_[to_id] = from_id;
        }
        return answer;
    }

    uint32_t secondMsfWeight() {
        uint32_t answer = UINT16_MAX;
        for (Edge& edge : this->edges_) {
            if (!edge.usedInBest) {
                continue;
            }
            edge.ignore = true;

            uint32_t answer_attempt = 0;
            std::fill(this->component_id_.begin(), this->component_id_.end(), Edge::kNone);
            for (const Edge& edge : this->edges_) {
                if (edge.ignore) {
                    continue;
                }
                uint16_t from_id = this->getComponentId(edge.from);
                uint16_t to_id = this->getComponentId(edge.to);
                if (from_id == to_id) {
                    continue;
                }
                answer_attempt += edge.mass;
                this->component_id_[to_id] = from_id;
            }
            if (answer_attempt < answer) {
                answer = answer_attempt;
            }

            edge.ignore = false;
        }
        return answer;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    uint16_t n, m;
    std::cin >> n >> m;
    Graph graph(n, m);
    for (uint16_t i = 0; i < m; ++i) {
        uint16_t from, to, mass;
        std::cin >> from >> to >> mass;
        --from;
        --to;
        graph.addEdge({from, to, mass});
    }

    std::cout << graph.msfWeight() << ' ' << graph.secondMsfWeight();
}