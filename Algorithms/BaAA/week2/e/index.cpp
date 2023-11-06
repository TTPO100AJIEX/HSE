#include <iostream>
#include <ios>
#include <vector>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n; std::cin >> n;
    std::vector <int> keys(n);
    for (unsigned int i = 0; i < n; ++i) std::cin >> keys[i];
    unsigned int k; std::cin >> k;
    for (unsigned int i = 0; i < k; ++i)
    {
        unsigned int index;
        std::cin >> index;
        --keys[index - 1];
    }
    for (unsigned int i = 0; i < n; ++i) std::cout << (keys[i] >= 0 ? "no" : "yes") << "\n";
    return 0;
}