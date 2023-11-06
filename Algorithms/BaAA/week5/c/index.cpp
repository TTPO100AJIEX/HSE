#include <ios>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n; std::cin >> n;
    std::vector <unsigned int> routers(n);
    for (unsigned int i = 0; i < n; ++i) std::cin >> routers[i];

    std::pair <unsigned int, unsigned int> cur = { *routers.begin(), 0 };
    for (std::vector <unsigned int>::iterator routerPrice = routers.begin() + 1; routerPrice != routers.end(); ++routerPrice)
    {
        unsigned int connectPrice; std::cin >> connectPrice;
        cur.second += connectPrice;
        unsigned int bestConnect = std::min(connectPrice, *routerPrice);
        cur = {
            std::min(cur.first + bestConnect, cur.second + *routerPrice),
            std::min(cur.first, cur.second)
        };
    }
    std::cout << cur.first;
}