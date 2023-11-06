#include <iostream>
#include <ios>
#include <vector>
#include <utility>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n; std::cin >> n;
    std::vector < std::pair <unsigned int, unsigned int> > data(n), res(n);
    for (unsigned int i = 0; i < n; i++) std::cin >> data[i].first >> data[i].second;
    
    
    for (unsigned int offset = 0; offset < 32; offset += 8)
    {
        unsigned int counter[256] = { 0 };
        for (unsigned int i = 0; i < n; ++i) ++counter[(data[i].second >> offset) & 255];
        unsigned int save = counter[0]; counter[0] = n;
        for (unsigned int i = 1; i < 256; i++)
        {
            std::swap(save, counter[i]);
            counter[i] = counter[i - 1] - counter[i];
        }
        res.resize(n);
        for (int i = n - 1; i >= 0; --i) res[--counter[(data[i].second >> offset) & 255]] = std::move(data[i]);
        data = std::move(res);
    }
    
    
    for (unsigned int i = 0; i < n; i++) std::cout << data[i].first << "   " << data[i].second << "\n";
    return 0;
}