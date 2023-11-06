#include <iostream>
#include <ios>
#include <vector>
#include <algorithm>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n; std::cin >> n;
    if (n == 0) return 0;
    std::vector <int> data(n), res(n);
    int min = 1e6, max = -1e6;
    for (unsigned int i = 0; i < n; ++i)
    {
        std::cin >> data[i];
        min = std::min(min, data[i]);
        max = std::max(max, data[i]);
    }


    unsigned int diff = max - min + 1;
    std::vector <unsigned int> counter(diff, 0);
    for (unsigned int i = 0; i < n; ++i) ++counter[data[i] - min];
    for (unsigned int i = 1; i < diff; ++i) counter[i] += counter[i - 1];
    for (int i = n - 1; i >= 0; i--) res[--counter[data[i] - min]] = data[i];


    for (unsigned int i = 0; i < n; ++i) std::cout << res[i] << " ";
    return 0;
}