#include <ios>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

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

    void dfsReversed(unsigned int vertex_index, unsigned int component_number,
                     std::vector<unsigned int>& component) {
        Vertex& vertex = this->vertexes_[vertex_index];
        if (vertex.component != 0) {
            return;
        }
        vertex.component = component_number;
        component.push_back(vertex_index);
        for (unsigned int sibling : vertex.edges_transposed) {
            this->dfsReversed(sibling, component_number, component);
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

    std::pair<unsigned int, unsigned int> getBestComponent() {
        std::vector<unsigned int> order;
        order.reserve(this->vertexes_.size());
        for (unsigned int i = 0; i < this->vertexes_.size(); ++i) {
            this->dfsDirect(i, order);
        }

        unsigned int component_number = 0;
        std::pair<unsigned int, unsigned int> best_component = {0, 0};  // { size, number }
        std::vector<unsigned int> component;
        for (std::vector<unsigned int>::const_reverse_iterator now = order.rbegin();
             now != order.rend(); ++now) {
            if (this->vertexes_[*now].component != 0) {
                continue;
            }
            component.clear();
            this->dfsReversed(*now, ++component_number, component);
            if (component.size() >= best_component.first && best_component.first != 0) {
                continue;
            }
            bool is_ok = std::all_of(
                component.begin(), component.end(), [&](const unsigned int vertex_index) {
                    const Vertex& vertex = this->vertexes_[vertex_index];
                    return std::all_of(
                        vertex.edges.begin(), vertex.edges.end(), [&](const unsigned int sibling) {
                            return (this->vertexes_[sibling].component == vertex.component);
                        });
                });
            if (is_ok) {
                best_component.first = component.size();
                best_component.second = component_number;
            }
        }
        return best_component;
    }

    void printComponent(unsigned int number, std::ostream& ostream) {
        for (unsigned int i = 0; i < vertexes_.size(); ++i) {
            if (this->vertexes_[i].component == number) {
                ostream << i + 1 << ' ';
            }
        }
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n, m, h;
    std::cin >> n >> m >> h;
    std::vector<unsigned int> hours(n);
    for (unsigned int i = 0; i < n; ++i) {
        std::cin >> hours[i];
    }
    Graph graph(n);
    for (unsigned int i = 0; i < m; ++i) {
        unsigned int from, to;
        std::cin >> from >> to;
        --from;
        --to;
        if (hours[from] % h == (hours[to] + 1) % h) {
            graph.addEdge(to, from);
        }
        if (hours[to] % h == (hours[from] + 1) % h) {
            graph.addEdge(from, to);
        }
    }

    const auto [size, number] = graph.getBestComponent();
    std::cout << size << '\n';
    graph.printComponent(number, std::cout);
}