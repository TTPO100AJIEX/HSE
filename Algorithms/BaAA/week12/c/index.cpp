#include <ios>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class Graph
{
private:
    struct Vertex
    {
        std::vector<unsigned int> edges;
        std::vector<unsigned int> edges_transposed;
        bool used = false;
        bool inComponent = false;
    };
    std::vector<Vertex> vertexes;

    void dfsDirect(unsigned int vertexIndex, std::vector<unsigned int>& order)
    {
        Vertex& vertex = this->vertexes[vertexIndex];
        if (vertex.used) return;
        vertex.used = true;
        for (unsigned int sibling : vertex.edges) this->dfsDirect(sibling, order);
        order.push_back(vertexIndex);
    }

    void dfsReversed(unsigned int vertexIndex, std::vector<unsigned int>& component)
    {
        Vertex& vertex = this->vertexes[vertexIndex];
        if (vertex.inComponent) return;
        vertex.inComponent = true;
        component.push_back(vertexIndex);
        for (unsigned int sibling : vertex.edges_transposed) this->dfsReversed(sibling, component);
    }

public:
    Graph(unsigned int vertexes)
    {
        this->vertexes.resize(vertexes);
    }

    void addEdge(unsigned int from, unsigned int to)
    {
        this->vertexes[from].edges.push_back(to);
        this->vertexes[to].edges_transposed.push_back(from);
    }

    std::vector< std::vector<unsigned int> > calculateComponents()
    {
        std::vector<unsigned int> order;
        order.reserve(this->vertexes.size());
        for (unsigned int i = 0; i < this->vertexes.size(); ++i) this->dfsDirect(i, order);
        
        std::vector< std::vector<unsigned int> > components;
        for (std::vector<unsigned int>::const_reverse_iterator now = order.rbegin(); now != order.rend(); ++now)
        {
            if (this->vertexes[*now].inComponent) continue;
            this->dfsReversed(*now, components.emplace_back());
        }
        return components;
    }
};



int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n, m;
    std::cin >> n >> m;

    std::vector<std::string> names(n);
    for (unsigned int i = 0; i < n; ++i)
    {
        unsigned int index; std::cin >> index;
        std::cin.ignore(1); std::getline(std::cin, names[index]);
    }

    Graph graph(n);
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int from, to;
        std::cin >> from >> to;
        graph.addEdge(from, to);
    }

    std::vector< std::vector<unsigned int> > components = graph.calculateComponents();
    for (std::vector<unsigned int>& component : components)
    {
        std::sort(component.begin(), component.end(), [&names](const unsigned int first, const unsigned int second) { return names[first] < names[second]; });
    }
    std::sort(components.begin(), components.end(), [&names](const std::vector <unsigned int>& first, const std::vector <unsigned int>& second)
    {
        unsigned int elements = std::min(first.size(), second.size());
        for (unsigned int i = 0; i < elements; ++i)
        {
            int comparison = names[first[i]].compare(names[second[i]]);
            if (comparison < 0) return true;
            if (comparison > 0) return false;
        }
        return (first.size() < second.size());
    });

    for (const std::vector <unsigned int>& component : components)
    {
        for (unsigned int poor_horsy : component) std::cout << names[poor_horsy] << '\n';
        std::cout << '\n';
    }
}