#include <iostream>
#include <ios>
#include <vector>
#include <cstring>

#define VALUE_LIMIT 1000000000

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    unsigned int n; std::cin >> n;
    std::vector <int> data(n);
    for (unsigned int i = 0; i < n; ++i) { std::cin >> data[i]; data[i] += VALUE_LIMIT; }


    std::vector <int> res(n);
    for (unsigned int offset = 0; offset < 32; offset += 8)
    {
        unsigned int counter[256] = { 0 };
        for (unsigned int i = 0; i < n; ++i) ++counter[(data[i] >> offset) & 255];
        for (unsigned int i = 1; i < 256; i++) counter[i] += counter[i - 1];
        for (int i = n - 1; i >= 0; --i) res[--counter[(data[i] >> offset) & 255]] = data[i];
        std::swap(data, res);
    }


    for (unsigned int i = 0; i < n; ++i) std::cout << data[i] - VALUE_LIMIT << " ";
    return 0;
}