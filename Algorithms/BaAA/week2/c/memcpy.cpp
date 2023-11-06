#include <iostream>
#include <ios>
#include <vector>
#include <cstring>

struct Drone
{
    unsigned int id;
    unsigned int weight;
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n; std::cin >> n;
    std::vector <Drone> data(n);
    for (unsigned int i = 0; i < n; i++) std::cin >> data[i].id >> data[i].weight;
    
    
    for (unsigned int offset = 0; offset < 32; offset += 8)
    {
        std::vector<Drone> counter[256];
        for (unsigned int i = 0; i < n; ++i) counter[(data[i].weight >> offset) & 255].push_back(data[i]);
        for (int i = 255, curIndex = 0; i >= 0; i--)
        {
            if (counter[i].size() != 0)
            {
                std::memcpy(&data[curIndex], counter[i].data(), counter[i].size() * sizeof(Drone));
                curIndex += counter[i].size();
            }
        }
    }
    
    for (unsigned int i = 0; i < n; i++) std::cout << data[i].id << "   " << data[i].weight << "\n";
    return 0;
}