#include <iostream>
#include <ios>
#include <vector>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n; std::cin >> n;
    if (n == 0) return 0;
    std::vector <int> data(n), res(n);
    for (unsigned int i = 0; i < n; ++i)  { std::cin >> data[i]; data[i] += 1000000; }

    unsigned int counter[2000001] = { 0 };
    for (unsigned int i = 0; i < n; ++i) ++counter[data[i]];
    for (unsigned int i = 1; i < 2000001; ++i) counter[i] += counter[i - 1];
    for (int i = n - 1; i >= 0; --i) res[--counter[data[i]]] = data[i];


    for (unsigned int i = 0; i < n; ++i) std::cout << res[i] - 1000000 << " ";
    return 0;
}