#include <ios>
#include <iostream>
#include <cstdint>
#include <vector>
#include <utility>
#include <algorithm>
#include <unordered_set>

class Graph {
private:
    constexpr static uint16_t kInfinity = UINT16_MAX;
    struct Vertex {
        std::vector<uint16_t> edges;
        std::vector<uint16_t> edges_transposed;
        bool used = false;
        uint16_t component = kInfinity;
        uint16_t level = kInfinity;
    };
    std::vector<Vertex> vertexes_;

    void dfsDirect(uint16_t vertex_index, std::vector<uint16_t>& order) {
        Vertex& vertex = this->vertexes_[vertex_index];
        if (vertex.used) {
            return;
        }
        vertex.used = true;
        for (uint16_t sibling : vertex.edges) {
            this->dfsDirect(sibling, order);
        }
        order.push_back(vertex_index);
    }

    void dfsReversed(uint16_t vertex_index, uint16_t component_number,
                     std::vector<uint16_t>& component) {
        Vertex& vertex = this->vertexes_[vertex_index];
        if (vertex.component != kInfinity) {
            return;
        }
        vertex.component = component_number;
        component.push_back(vertex_index);
        for (uint16_t sibling : vertex.edges_transposed) {
            this->dfsReversed(sibling, component_number, component);
        }
    }

    void dfsLevels(uint16_t vertex_index, uint16_t level = 1) {
        Vertex& vertex = this->vertexes_[vertex_index];
        if (vertex.level != 0) {
            vertex.level = std::max(vertex.level, level);
            return;
        }
        vertex.level = std::max(vertex.level, level);
        for (uint16_t sibling : vertex.edges) {
            this->dfsLevels(sibling, level + 1);
        }
    }

public:
    Graph(uint16_t vertexes) {
        this->vertexes_.resize(vertexes);
    }

    void addEdge(uint16_t from, uint16_t to) {
        this->vertexes_[from].edges.push_back(to);
        this->vertexes_[to].edges_transposed.push_back(from);
    }

    std::vector<std::pair<uint16_t, uint16_t> > calculateMinimalConnectionSet() {
        std::vector<uint16_t> order;
        order.reserve(this->vertexes_.size());
        for (uint16_t i = 0; i < this->vertexes_.size(); ++i) {
            this->dfsDirect(i, order);
        }

        std::vector<std::pair<uint16_t, uint16_t> > answer;
        uint16_t component_number = 0;
        std::vector<uint16_t> component;
        for (std::vector<uint16_t>::const_reverse_iterator now = order.rbegin();
             now != order.rend(); ++now) {
            if (this->vertexes_[*now].component != kInfinity) {
                continue;
            }
            component.clear();
            this->dfsReversed(*now, component_number++, component);
            if (component.size() == 1) {
                continue;
            }
            for (uint16_t i = 0; i < component.size() - 1; ++i) {
                answer.emplace_back(component[i], component[i + 1]);
            }
            answer.emplace_back(component[component.size() - 1], component[0]);
        }

        std::vector<uint16_t>& component_vertexes = order;
        component_vertexes.resize(component_number);
        std::fill(component_vertexes.begin(), component_vertexes.end(), kInfinity);
        for (uint16_t vertex_index = 0; vertex_index < this->vertexes_.size(); ++vertex_index) {
            Vertex& vertex = this->vertexes_[vertex_index];
            if (component_vertexes[vertex.component] == kInfinity) {
                component_vertexes[vertex.component] = vertex_index;
                continue;
            }
            Vertex& component_vertex = this->vertexes_[component_vertexes[vertex.component]];
            component_vertex.edges.insert(component_vertex.edges.end(), vertex.edges.begin(),
                                          vertex.edges.end());
            vertex.edges.clear();
        }

        std::unordered_set<uint16_t> filter;
        for (Vertex& vertex : this->vertexes_) {
            filter.clear();
            for (uint16_t& sibling : vertex.edges) {
                if (vertex.component == this->vertexes_[sibling].component) {
                    continue;
                }
                filter.insert(component_vertexes[this->vertexes_[sibling].component]);
            }
            vertex.edges.clear();
            vertex.edges.insert(vertex.edges.end(), filter.begin(), filter.end());
        }

        for (uint16_t vertex_index = 0; vertex_index < this->vertexes_.size(); ++vertex_index) {
            Vertex& vertex = this->vertexes_[vertex_index];
            if (vertex.edges.empty()) {
                continue;
            }
            for (Vertex& vertex : this->vertexes_) {
                vertex.level = 0;
            }
            dfsLevels(vertex_index);
            vertex.edges.erase(std::remove_if(vertex.edges.begin(), vertex.edges.end(),
                                              [&](uint16_t sibling) {
                                                  return this->vertexes_[sibling].level > 2;
                                              }),
                               vertex.edges.end());
        }

        for (uint16_t vertex_index = 0; vertex_index < this->vertexes_.size(); ++vertex_index) {
            for (uint16_t sibling : this->vertexes_[vertex_index].edges) {
                answer.emplace_back(vertex_index, sibling);
            }
        }

        return answer;
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
        std::cin >> from >> to;
        graph.addEdge(from - 1, to - 1);
    }

    std::vector<std::pair<uint16_t, uint16_t> > answer = graph.calculateMinimalConnectionSet();
    std::cout << answer.size() << '\n';
    for (const std::pair<uint16_t, uint16_t>& edge : answer) {
        std::cout << edge.first + 1 << ' ' << edge.second + 1 << '\n';
    }
}