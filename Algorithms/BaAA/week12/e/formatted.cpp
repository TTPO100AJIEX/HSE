#include <ios>
#include <iostream>
#include <cstdint>
#include <utility>
#include <vector>

class Graph {
private:
    struct Vertex {
        std::vector<unsigned int> edges;
        std::vector<unsigned int> edges_transposed;
        bool used = false;
        unsigned int component = 0;
    };
    std::vector<Vertex> vertexes_;

    void dfsDirect(unsigned int vertex_index, std::vector<unsigned int>& order) {
        Vertex& vertex = this->vertexes_[vertex_index];
        if (vertex.used) {
            return;
        }
        vertex.used = true;
        for (unsigned int sibling : vertex.edges) {
            this->dfsDirect(sibling, order);
        }
        order.push_back(vertex_index);
    }

    void dfsReversed(unsigned int vertex_index, unsigned int component) {
        Vertex& vertex = this->vertexes_[vertex_index];
        if (vertex.component != 0) {
            return;
        }
        vertex.component = component;
        for (unsigned int sibling : vertex.edges_transposed) {
            this->dfsReversed(sibling, component);
        }
    }

public:
    Graph(unsigned int vertexes) {
        this->vertexes_.resize(vertexes);
    }

    void addEdge(unsigned int from, unsigned int to) {
        this->vertexes_[from].edges.push_back(to);
        this->vertexes_[to].edges_transposed.push_back(from);
    }

    std::pair<unsigned int, std::vector<Vertex> > calculateComponents() {
        std::vector<unsigned int> order;
        order.reserve(this->vertexes_.size());
        for (unsigned int i = 0; i < this->vertexes_.size(); ++i) {
            this->dfsDirect(i, order);
        }

        unsigned int component_number = 0;
        for (std::vector<unsigned int>::const_reverse_iterator now = order.rbegin();
             now != order.rend(); ++now) {
            if (this->vertexes_[*now].component != 0) {
                continue;
            }
            this->dfsReversed(*now, ++component_number);
        }
        return {component_number, std::move(this->vertexes_)};
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n, m;
    std::cin >> n >> m;
    Graph graph(n);
    for (unsigned int i = 0; i < m; ++i) {
        unsigned int from, to;
        std::cin >> from >> to;
        graph.addEdge(from - 1, to - 1);
    }
    const auto [componentsAmount, components] = graph.calculateComponents();

    std::cout << componentsAmount << '\n';
    for (const auto& component : components) {
        std::cout << component.component << ' ';
    }
}