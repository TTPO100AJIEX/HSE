#include <iostream>
#include <inttypes.h>
#include <vector>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    unsigned int n, k; std::cin >> n >> k;
    std::vector <unsigned int> lengths(n);
    uint64_t l = 0, r = 0;
    for (unsigned int i = 0; i < n; i++)
    {
        std::cin >> lengths[i];
        r += lengths[i];
    }
    r = r / k + 1;
    while (r - l > 1)
    {
        uint64_t length = ((r + l) / 2);
        unsigned int amount = 0;
        for (unsigned int i = 0; i < n; i++) amount += (lengths[i] / length);
        if (amount >= k) l = length;
        else r = length;
    }
    std::cout << l;
}