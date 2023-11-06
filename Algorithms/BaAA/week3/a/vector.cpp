#include <ios>
#include <iostream>
#include <vector>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n; std::cin >> n;
    std::vector<int> data(n);
    for (unsigned int i = 0; i < n; i++)
    {
        std::cin >> data[i];
        if (i != 0 && data[i] > data[(i - 1) >> 1]) { std::cout << i - 1; return 0; }
    }
    std::cout << n - 1;
}