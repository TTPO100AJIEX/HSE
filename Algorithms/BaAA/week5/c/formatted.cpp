#include <ios>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n;
    std::cin >> n;
    std::vector<unsigned int> routers(n);
    for (unsigned int i = 0; i < n; ++i) {
        std::cin >> routers[i];
    }

    std::pair<unsigned int, unsigned int> cur = {*routers.begin(), 0};
    for (std::vector<unsigned int>::iterator router_price = routers.begin() + 1;
         router_price != routers.end(); ++router_price) {
        unsigned int connect_price;
        std::cin >> connect_price;
        cur.second += connect_price;
        cur = {std::min(cur.first + std::min(connect_price, *router_price),
                        cur.second + *router_price),
               std::min(cur.first, cur.second)};
    }
    std::cout << cur.first;
}