#include <ios>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

void radixSort(std::vector< std::pair<unsigned int, int> >& data)
{
    std::vector< std::pair<unsigned int, int> > res(data.size());
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
    std::vector< std::pair<unsigned int, int> > events(n << 1);
    for (std::vector< std::pair<unsigned int, int> >::iterator now = events.begin(); now != events.end(); now += 2)
    {
        std::vector< std::pair<unsigned int, int> >::iterator next = now + 1;
        std::cin >> now->first >> next->first; now->second = 1; next->second = -1; next->first++;
    }
    radixSort(events);

    std::vector< std::pair<unsigned int, unsigned int> > points;
    points.emplace_back(0, 0);
    for (std::vector< std::pair<unsigned int, int> >::iterator event = events.begin(); event != events.end(); ++event)
    {
        if (points.back().first != event->first) points.emplace_back(event->first, points.back().second);
        points.back().second += event->second;
    }
    
    char command; std::cin >> command;
    while (command != '!')
    {
        unsigned int time; std::cin >> time;
        std::vector < std::pair<unsigned int, unsigned int> >::iterator point = std::upper_bound(points.begin(), points.end(), time,
            [](const unsigned int value, const std::pair<unsigned int, unsigned int>& point) { return value < point.first; }) - 1;
        std::cout << "! " << point->second << std::endl;
        std::cin >> command;
    }
}