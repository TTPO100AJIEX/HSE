#include <ios>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n, m; std::cin >> n >> m;
    std::vector<std::string> names(n);
    std::vector< std::vector<unsigned int> > edges(n);
    for (unsigned int i = 0; i < n; ++i)
    {
        unsigned int index; std::cin >> index; std::cin.ignore(1);
        std::getline(std::cin, names[index]);
    }
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int from, to;
        std::cin >> from >> to;
        edges[from].push_back(to);
        edges[to].push_back(from);
    }

    unsigned int number;
    std::cin >> number;
    std::vector<unsigned int> current, next;
    std::vector<bool> visited(n, false);
    next.push_back(number);
    visited[number] = true;
    while (!next.empty())
    {
        std::swap(current, next);
        next.clear();
        std::sort(current.begin(), current.end(), [&](unsigned int first, unsigned int second) { return names[first] < names[second]; });
        for (unsigned int to_print : current)
        {
            std::cout << names[to_print] << '\n';
            for (unsigned int sibling : edges[to_print])
            {
                if (visited[sibling]) continue;
                visited[sibling] = true;
                next.push_back(sibling);
            }
        }
    }

    return 0;
}