#include <ios>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <utility>

std::vector <unsigned int> weights;
std::vector < std::map <unsigned int, unsigned int> > cache;
unsigned int f(unsigned int i, unsigned int j)
{
    if (i == 0 || j == 0) return 0;
    std::map <unsigned int, unsigned int>::iterator cached = cache[i - 1].find(j);
    if (cached != cache[i - 1].end()) return cached->second;

    unsigned int res;
    if (j >= weights[i - 1]) res = std::max(f(i - 1, j), f(i - 1, j - weights[i - 1]) + weights[i - 1]);
    else res = f(i - 1, j);
    cache[i - 1].insert({ j, res });
    return res;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n, w; std::cin >> n >> w;
    weights.resize(n);
    cache.resize(n);
    for (unsigned int i = 0; i < n; ++i) std::cin >> weights[i];
    std::sort(weights.begin(), weights.end(), [](unsigned int first, unsigned int second) { return second < first; });

    std::cout << f(n, w) << "\n";

    std::vector <unsigned int> ans;
    while (n != 0)
    {
        if (f(n, w) == f(n - 1, w))
        {
            n--;
        }
        else
        {
            ans.push_back(weights[n - 1]);
            w -= weights[--n];
        }
    }
    std::cout << ans.size() << "\n";
    for (unsigned int i = 0; i < ans.size(); ++i) std::cout << ans[i] << " ";
}