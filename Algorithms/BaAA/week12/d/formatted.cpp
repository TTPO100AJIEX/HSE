#include <ios>
#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <algorithm>

class Graph {
private:
    struct Vertex {
        std::vector<uint16_t> edges;
        bool isSrc = true;
        bool used = false;
    };
    std::vector<Vertex> vertexes_;
    uint16_t srces_counter_;

    void dfs(uint16_t vertex_index, std::vector<uint16_t>& order) {
        Vertex& vertex = this->vertexes_[vertex_index];
        if (vertex.used) {
            return;
        }
        vertex.used = true;
        for (uint16_t sibling : vertex.edges) {
            this->dfs(sibling, order);
        }
        order.push_back(vertex_index);
    }

public:
    Graph(uint16_t vertexes) {
        this->vertexes_.resize(vertexes);
        this->srces_counter_ = vertexes;
    }

    void addEdge(uint16_t from, uint16_t to) {
        this->vertexes_[from].edges.push_back(to);
        if (this->vertexes_[to].isSrc) {
            --this->srces_counter_;
            this->vertexes_[to].isSrc = false;
        }
    }

    template <typename SortFunction>
    std::vector<uint16_t> topSort(SortFunction sorter) {
        std::vector<uint16_t> srces;
        srces.reserve(this->srces_counter_);
        for (unsigned int i = 0; i < this->vertexes_.size(); ++i) {
            if (this->vertexes_[i].isSrc) {
                srces.push_back(i);
            }
        }

        std::sort(srces.begin(), srces.end(), sorter);
        for (Vertex& vertex : vertexes_) {
            std::sort(vertex.edges.begin(), vertex.edges.end(), sorter);
        }

        std::vector<uint16_t> order;
        order.reserve(this->vertexes_.size());
        for (uint16_t src : srces) {
            this->dfs(src, order);
        }
        return order;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n, m;
    std::cin >> n >> m;

    std::vector<std::string> names(n);
    for (uint16_t i = 0; i < n; ++i) {
        uint16_t index;
        std::cin >> index;
        std::cin.ignore(1);
        std::getline(std::cin, names[--index]);
    }

    Graph graph(n);
    for (unsigned int i = 0; i < m; ++i) {
        uint16_t from, to;
        std::cin >> from >> to;
        graph.addEdge(from - 1, to - 1);
    }

    std::vector<uint16_t> order =
        graph.topSort([&names](const uint16_t first, const uint16_t second) {
            return names[first] < names[second];
        });
    for (std::vector<uint16_t>::const_reverse_iterator now = order.rbegin(); now != order.rend();
         ++now) {
        std::cout << names[*now] << '\n';
    }
}