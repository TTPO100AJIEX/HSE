#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    unsigned int n, m; std::cin >> n >> m;
    std::vector< std::pair<int, int> > points((n << 1) + 1);

    (*points.begin()) = { -(1 << 30), 0 };
    for (std::vector< std::pair<int, int> >::iterator now = points.begin() + 1, next = now + 1; now != points.end(); now += 2, next += 2)
    {
        std::cin >> now->first >> next->first; now->second = 1; next->second = -1; next->first++;
    }
    std::sort(points.begin() + 1, points.end());
    
    std::vector< std::pair<int, int> >::iterator lastPoint = points.begin();
    for (std::vector< std::pair<int, int> >::iterator point = points.begin() + 1; point != points.end(); ++point)
    {
        if (lastPoint->first != point->first)
        {
            point->second += lastPoint->second;
            (*(++lastPoint)) = *point;
            continue;
        }
        lastPoint->second += point->second;
    }
    
    for (unsigned int i = 0; i < m; ++i)
    {
        int point; std::cin >> point;
        std::vector < std::pair<int, int> >::iterator answer = std::upper_bound(points.begin(), lastPoint + 1, point,
            [](const int value, const std::pair<int, int>& point) { return value < point.first; }) - 1;
        std::cout << answer->second << " ";
    }
}