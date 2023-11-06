#include <ios>
#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>

struct Item {
    unsigned int weight;
    unsigned int price;
};
std::vector<Item> items;
std::vector<std::vector<int64_t> > cache;
int64_t solve(unsigned int i, unsigned int j) {
    // if (i == 0 || j == 0) return cache[i][j] = 0;
    if (cache[i][j] != -1) {
        return cache[i][j];
    }

    unsigned int weight = items[i - 1].weight, price = items[i - 1].price;
    if (j >= weight) {
        return cache[i][j] = std::max(solve(i - 1, j), solve(i - 1, j - weight) + price);
    } else {
        return cache[i][j] = solve(i - 1, j);
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    unsigned int n, w;
    std::cin >> n >> w;
    items.resize(n);
    for (Item& item : items) {
        std::cin >> item.weight;
    }
    for (Item& item : items) {
        std::cin >> item.price;
    }
    cache.resize(n + 1, std::vector<int64_t>(w + 1, -1));

    for (unsigned int i = 0; i <= n; i++) {
        cache[i][0] = 0;
    }
    for (unsigned int i = 0; i <= w; i++) {
        cache[0][i] = 0;
    }

    solve(n, w);

    unsigned int sum_weight = 0;
    int64_t sum_price = 0;
    std::vector<Item> taken_items;
    std::vector<Item>::reverse_iterator item_iter = items.rbegin();
    for (std::vector<std::vector<int64_t> >::reverse_iterator now = cache.rbegin();
         now != cache.rend() - 1; ++now, ++item_iter) {
        if ((*now)[w] == (*(now + 1))[w]) {
            continue;
        }
        w -= item_iter->weight;
        sum_weight += item_iter->weight;
        sum_price += item_iter->price;
        taken_items.push_back(*item_iter);
    }

    std::cout << sum_price << "\n" << sum_weight << "\n" << taken_items.size() << "\n";
    for (const Item& item : taken_items) {
        std::cout << item.weight << " ";
    }
    std::cout << "\n";
    for (const Item& item : taken_items) {
        std::cout << item.price << " ";
    }
}
