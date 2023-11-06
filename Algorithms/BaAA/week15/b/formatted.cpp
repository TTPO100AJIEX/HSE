#include <ios>
#include <iostream>
#include <climits>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <queue>
#include <map>
#include <utility>

struct Edge {
    unsigned int to;
    unsigned int reverse_edge;
    int cur_flow = 0;
    unsigned int max_flow;

    Edge(unsigned int to, unsigned int reverse_edge, int max_flow)
        : to(to), reverse_edge(reverse_edge), max_flow(max_flow) {
    }
};
std::vector<std::vector<Edge> > edges;
std::vector<unsigned int> distances;
constexpr unsigned int kInf = (UINT_MAX >> 1);

void findDistances(unsigned int from) {
    distances[from] = 0;
    static std::queue<unsigned int> to_update;
    to_update.push(from);
    while (!to_update.empty()) {
        unsigned int updating = to_update.front();
        to_update.pop();
        for (const Edge& edge : edges[updating]) {
            if (edge.cur_flow >= static_cast<int>(edge.max_flow) ||
                distances[edge.to] <= distances[updating] + 1) {
                continue;
            }
            distances[edge.to] = distances[updating] + 1;
            to_update.push(edge.to);
        }
    }
}
unsigned int findFlow(unsigned int from, unsigned int to, unsigned int flow = (UINT_MAX >> 1)) {
    if (from == to || flow == 0) {
        return flow;
    }
    for (Edge& edge : edges[from]) {
        if (distances[edge.to] != distances[from] + 1) {
            continue;
        }
        unsigned int add_flow = findFlow(
            edge.to, to,
            std::min(flow,
                     static_cast<unsigned int>(static_cast<int>(edge.max_flow) - edge.cur_flow)));
        if (add_flow == 0) {
            continue;
        }
        edge.cur_flow += add_flow;
        edges[edge.to][edge.reverse_edge].cur_flow -= add_flow;
        return add_flow;
    }
    return 0;
}
uint64_t maxFlow(unsigned int from, unsigned int to) {
    uint64_t answer = 0;
    std::fill(distances.begin(), distances.end(), kInf);
    findDistances(from);
    while (distances[to] != kInf) {
        unsigned int flow = findFlow(from, to);
        while (flow != 0) {
            answer += flow;
            flow = findFlow(from, to);
        }
        std::fill(distances.begin(), distances.end(), kInf);
        findDistances(from);
    }
    return answer;
}

void addEdge(unsigned int from, unsigned int to) {
    edges[from].emplace_back(to, edges[to].size(), 1);
    edges[to].emplace_back(from, edges[from].size() - 1, 0);
}
std::map<std::pair<int, int>, unsigned int> vertexes;
unsigned int getVertexIndex(int x, int y) {
    if (vertexes.find({x, y}) == vertexes.end()) {
        addEdge(vertexes.size() << 1, (vertexes.size() << 1) + 1);
        vertexes.insert_or_assign({x, y}, vertexes.size() << 1);
    }
    return vertexes.find({x, y})->second;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n;
    std::cin >> n;
    edges.resize((n + 1) << 2);
    distances.resize(edges.size());
    for (unsigned int i = 0; i < n; i++) {
        int x1, y1, x2, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;
        unsigned int v1 = getVertexIndex(x1, y1), v2 = getVertexIndex(x2, y2);
        addEdge(v1 + 1, v2);
        addEdge(v2 + 1, v1);
    }

    int x1, y1, x2, y2;
    std::cin >> x1 >> y1 >> x2 >> y2;
    std::cout << maxFlow(getVertexIndex(x1, y1) + 1, getVertexIndex(x2, y2));
}