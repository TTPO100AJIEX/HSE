#include <ios>
#include <iostream>
#include <cstdint>
#include <vector>
#include <utility>
#include <algorithm>
#include <unordered_set>

class Graph
{
private:
    constexpr static uint16_t INFINITY = UINT16_MAX;
    struct Vertex
    {
        std::vector<uint16_t> edges;
        std::vector<uint16_t> edges_transposed;
        bool used = false;
        uint16_t component = INFINITY;
        uint16_t level = INFINITY;
    };
    std::vector<Vertex> vertexes;

    void dfsDirect(uint16_t vertexIndex, std::vector<uint16_t>& order)
    {
        Vertex& vertex = this->vertexes[vertexIndex];
        if (vertex.used) return;
        vertex.used = true;
        for (uint16_t sibling : vertex.edges) this->dfsDirect(sibling, order);
        order.push_back(vertexIndex);
    }

    void dfsReversed(uint16_t vertexIndex, uint16_t componentNumber, std::vector<uint16_t>& component)
    {
        Vertex& vertex = this->vertexes[vertexIndex];
        if (vertex.component != INFINITY) return;
        vertex.component = componentNumber;
        component.push_back(vertexIndex);
        for (uint16_t sibling : vertex.edges_transposed) this->dfsReversed(sibling, componentNumber, component);
    }

    void dfsLevels(uint16_t vertexIndex, uint16_t level = 1)
    {
        Vertex& vertex = this->vertexes[vertexIndex];
        if (vertex.level != 0) { vertex.level = std::max(vertex.level, level); return; }
        vertex.level = std::max(vertex.level, level);
        for (uint16_t sibling : vertex.edges) this->dfsLevels(sibling, level + 1);
    }

public:
    Graph(uint16_t vertexes)
    {
        this->vertexes.resize(vertexes);
    }

    void addEdge(uint16_t from, uint16_t to)
    {
        this->vertexes[from].edges.push_back(to);
        this->vertexes[to].edges_transposed.push_back(from);
    }

    std::vector< std::pair<uint16_t, uint16_t> > calculateMinimalConnectionSet()
    {
        std::vector<uint16_t> order;
        order.reserve(this->vertexes.size());
        for (uint16_t i = 0; i < this->vertexes.size(); ++i) this->dfsDirect(i, order);
        
        std::vector< std::pair<uint16_t, uint16_t> > answer;
        uint16_t componentNumber = 0;
        std::vector<uint16_t> component;
        for (std::vector<uint16_t>::const_reverse_iterator now = order.rbegin(); now != order.rend(); ++now)
        {
            if (this->vertexes[*now].component != INFINITY) continue;
            component.clear();
            this->dfsReversed(*now, componentNumber++, component);
            if (component.size() == 1) continue;
            for (uint16_t i = 0; i < component.size() - 1; ++i) answer.emplace_back(component[i], component[i + 1]);
            answer.emplace_back(component[component.size() - 1], component[0]);
        }


        std::vector<uint16_t>& componentVertexes = order;
        componentVertexes.resize(componentNumber);
        std::fill(componentVertexes.begin(), componentVertexes.end(), INFINITY);
        for (uint16_t vertexIndex = 0; vertexIndex < this->vertexes.size(); ++vertexIndex)
        {
            Vertex& vertex = this->vertexes[vertexIndex];
            if (componentVertexes[vertex.component] == INFINITY) { componentVertexes[vertex.component] = vertexIndex; continue; }
            Vertex& componentVertex = this->vertexes[componentVertexes[vertex.component]];
            componentVertex.edges.insert(componentVertex.edges.end(), vertex.edges.begin(), vertex.edges.end());
            vertex.edges.clear();
        }

        std::unordered_set<uint16_t> filter;
        for (Vertex& vertex : this->vertexes)
        {
            filter.clear();
            for (uint16_t& sibling : vertex.edges)
            {
                if (vertex.component == this->vertexes[sibling].component) continue;
                filter.insert(componentVertexes[this->vertexes[sibling].component]);
            }
            vertex.edges.clear();
            vertex.edges.insert(vertex.edges.end(), filter.begin(), filter.end());
        }

        for (uint16_t vertexIndex = 0; vertexIndex < this->vertexes.size(); ++vertexIndex)
        {
            Vertex& vertex = this->vertexes[vertexIndex];
            if (vertex.edges.size() == 0) continue;
            for (Vertex& vertex : this->vertexes) vertex.level = 0;
            dfsLevels(vertexIndex);
            vertex.edges.erase(std::remove_if(vertex.edges.begin(), vertex.edges.end(), [&](uint16_t sibling) { return this->vertexes[sibling].level > 2; }), vertex.edges.end());
        }

        for (uint16_t vertexIndex = 0; vertexIndex < this->vertexes.size(); ++vertexIndex)
        {
            for (uint16_t sibling : this->vertexes[vertexIndex].edges) answer.emplace_back(vertexIndex, sibling);
        }

        return answer;
    }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    uint16_t n, m;
    std::cin >> n >> m;
    Graph graph(n);
    for (uint16_t i = 0; i < m; ++i)
    {
        uint16_t from, to;
        std::cin >> from >> to;
        graph.addEdge(from - 1, to - 1);
    }
    
    std::vector< std::pair<uint16_t, uint16_t> > answer = graph.calculateMinimalConnectionSet();
    std::cout << answer.size() << '\n';
    for (const std::pair<uint16_t, uint16_t>& edge : answer) std::cout << edge.first + 1 << ' ' << edge.second + 1 << '\n';
}