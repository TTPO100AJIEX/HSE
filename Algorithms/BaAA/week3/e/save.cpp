#include <ios>
#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

void radixSort(std::vector< std::pair<unsigned int, bool> >& data)
{
    std::vector< std::pair<unsigned int, bool> > res(data.size());
    for (unsigned int offset = 0; offset < 32; offset += 8)
    {
        unsigned int counter[256] = { 0 };
        for (unsigned int i = 0; i < data.size(); ++i) ++counter[(data[i].first >> offset) & 255];
        for (unsigned int i = 1; i < 256; ++i) counter[i] += counter[i - 1];
        for (int i = data.size() - 1; i >= 0; --i) res[--counter[(data[i].first >> offset) & 255]] = data[i];
        std::swap(data, res);
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n; std::cin >> n;
    std::vector < std::pair<unsigned int, bool> > events(n << 1);
    for (unsigned int i = 0; i < events.size(); i += 2)
    {
        events[i].second = true; events[i + 1].second = false;
        std::cin >> events[i].first >> events[i + 1].first;
    }
    radixSort(events);

    std::vector < std::tuple<unsigned int, unsigned int, unsigned int> > points;
    points.emplace_back(0, 0, 0);
    for (unsigned int i = 0; i < events.size(); ++i)
    {
        if (std::get<0>(points[points.size() - 1]) != events[i].first)
            points.emplace_back(events[i].first, std::get<1>(points[points.size() - 1]), std::get<1>(points[points.size() - 1]));
        std::get<1>(points[points.size() - 1]) += (events[i].second ? 1 : -1);
        std::get<2>(points[points.size() - 1]) += events[i].second;
    }
    
    char command; std::cin >> command;
    while (command != '!')
    {
        unsigned int time; std::cin >> time;
        std::vector < std::tuple<unsigned int, unsigned int, unsigned int> >::iterator point = std::upper_bound(points.begin(), points.end(), time,
            [](unsigned int value, const std::tuple<unsigned int, unsigned int, unsigned int>& point) { return value < std::get<0>(point); }) - 1;
        if (std::get<0>(*point) == time)
        {
            std::cout << "! " << std::get<2>(*point) << std::endl;
        }
        else
        {
            std::cout << "! " << std::get<1>(*point) << std::endl;
        }

        std::cin >> command;
    }
}