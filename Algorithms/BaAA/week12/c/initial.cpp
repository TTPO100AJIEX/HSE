#include <ios>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

std::vector< std::vector <unsigned int> > edges;
std::vector< std::vector <unsigned int> > edges_transposed;
std::vector<bool> used;
std::vector<unsigned int> order;
unsigned int componentNumber = 0;
std::vector< std::vector <unsigned int> > components;

void dfs1(unsigned int vertex)
{
    if (used[vertex]) return;
    used[vertex] = true;
    for (unsigned int sibling : edges[vertex]) dfs1(sibling);
    order.push_back(vertex);
}

void dfs2(unsigned int vertex)
{
    if (used[vertex]) return;
    components[components.size() - 1].push_back(vertex);
    used[vertex] = true;
    for (unsigned int sibling : edges_transposed[vertex]) dfs2(sibling);
}


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

    edges.resize(n);
    edges_transposed.resize(n);
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int from, to;
        std::cin >> from >> to;
        edges[from].push_back(to);
        edges_transposed[to].push_back(from);
    }

    used.resize(n, false);
    order.reserve(n);
    for (unsigned int i = 0; i < n; ++i) dfs1(i);

    std::fill(used.begin(), used.end(), false);
    for (unsigned int i = 0; i < n; ++i)
    {
        if (used[order[n - i - 1]]) continue;
        components.emplace_back();
        dfs2(order[n - i - 1]);
        std::sort(components[components.size() - 1].begin(), components[components.size() - 1].end(), [&names](const unsigned int first, const unsigned int second)
        {
            return names[first] < names[second];
        });
    }
    std::sort(components.begin(), components.end(), [&names](const std::vector <unsigned int>& first, const std::vector <unsigned int>& second)
    {
        return names[first[0]] < names[second[0]];
    });

    for (const std::vector <unsigned int>& component : components)
    {
        for (unsigned int poor_horsy : component) std::cout << names[poor_horsy] << '\n';
        std::cout << '\n';
    }
}