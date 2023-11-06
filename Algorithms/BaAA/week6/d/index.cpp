#include <ios>
#include <iostream>
#include <cstdint>
#include <vector>
#include <utility>
#include <algorithm>

struct Item
{
    unsigned int weight;
    unsigned int price;

    bool operator< (const Item& other) const
    {
        uint64_t mul1 = this->weight * other.price, mul2 = other.weight * this->price;
        if (mul1 == mul2) return this->weight > other.weight;
        return mul1 < mul2;
    }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    unsigned int n, w; std::cin >> n >> w;
    std::vector <Item> items(n);
    for (Item& item : items) std::cin >> item.weight;
    for (Item& item : items) std::cin >> item.price;
    std::sort(items.begin(), items.end());

    unsigned int sumWeight = 0;
    uint64_t sumPrice = 0;
    std::vector <Item> takenItems;
    for (const Item& item : items)
    {
        if (item.weight > w) continue;
        w -= item.weight;
        sumWeight += item.weight;
        sumPrice += item.price;
        takenItems.push_back(item);
    }

    std::cout << sumPrice << "\n" << sumWeight << "\n" << takenItems.size() << "\n";
    for (const Item& item : takenItems) std::cout << item.weight << " ";
    std::cout << "\n";
    for (const Item& item : takenItems) std::cout << item.price << " ";
}