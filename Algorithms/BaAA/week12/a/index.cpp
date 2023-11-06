#include <ios>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

class Graph
{
private:
    struct Vertex
    {
        std::vector<unsigned int> edges;
        std::vector<unsigned int> edges_transposed;
        bool used = false;
        unsigned int component = 0;
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

    void dfsReversed(unsigned int vertexIndex, unsigned int componentNumber, std::vector<unsigned int>& component)
    {
        Vertex& vertex = this->vertexes[vertexIndex];
        if (vertex.component != 0) return;
        vertex.component = componentNumber;
        component.push_back(vertexIndex);
        for (unsigned int sibling : vertex.edges_transposed) this->dfsReversed(sibling, componentNumber, component);
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

    std::pair<unsigned int, unsigned int> getBestComponent()
    {
        std::vector<unsigned int> order;
        order.reserve(this->vertexes.size());
        for (unsigned int i = 0; i < this->vertexes.size(); ++i) this->dfsDirect(i, order);

        unsigned int componentNumber = 0;
        std::pair<unsigned int, unsigned int> bestComponent = { 0, 0 }; // { size, number }
        std::vector<unsigned int> component;
        for (std::vector<unsigned int>::const_reverse_iterator now = order.rbegin(); now != order.rend(); ++now)
        {
            if (this->vertexes[*now].component != 0) continue;
            component.clear();
            this->dfsReversed(*now, ++componentNumber, component);
            if (component.size() >= bestComponent.first && bestComponent.first != 0) continue;
            bool isOk = std::all_of(component.begin(), component.end(), [&](const unsigned int vertexIndex)
            {
                const Vertex& vertex = this->vertexes[vertexIndex];
                return std::all_of(vertex.edges.begin(), vertex.edges.end(), [&](const unsigned int sibling)
                {
                    return (this->vertexes[sibling].component == vertex.component);
                });
            });
            if (isOk)
            {
                bestComponent.first = component.size();
                bestComponent.second = componentNumber;
            }
        }
        return bestComponent;
    }

    void printComponent(unsigned int number, std::ostream& ostream)
    {
        for (unsigned int i = 0; i < vertexes.size(); ++i)
        {
            if (this->vertexes[i].component == number) ostream << i + 1 << ' ';
        }
    }
};



int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n, m, h;
    std::cin >> n >> m >> h;
    std::vector <unsigned int> hours(n);
    for (unsigned int i = 0; i < n; ++i) std::cin >> hours[i];
    Graph graph(n);
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int from, to;
        std::cin >> from >> to;
        --from; --to;
        if (hours[from] % h == (hours[to] + 1) % h) graph.addEdge(to, from);
        if (hours[to] % h == (hours[from] + 1) % h) graph.addEdge(from, to);
    }

    const auto [ size, number ] = graph.getBestComponent();
    std::cout << size << '\n';
    graph.printComponent(number, std::cout);
}