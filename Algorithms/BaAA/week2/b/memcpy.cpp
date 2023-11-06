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
    int* data = (int*)(std::malloc(n * sizeof(int)));
    for (unsigned int i = 0; i < n; ++i) { std::cin >> data[i]; data[i] += VALUE_LIMIT; }


    for (unsigned int offset = 0; offset < 32; offset += 8)
    {
        std::vector<int> counter[256];
        for (unsigned int i = 0; i < n; ++i) counter[(data[i] >> offset) & 255].push_back(data[i]);
        for (unsigned int i = 0, curIndex = 0; i < 256; ++i)
        {
            if (counter[i].size() != 0)
            {
                std::memcpy(&data[curIndex], counter[i].data(), counter[i].size() * sizeof(int));
                curIndex += counter[i].size();
            }
        }
    }


    for (unsigned int i = 0; i < n; ++i) std::cout << data[i] - VALUE_LIMIT << " ";
    free(data);
    return 0;
}