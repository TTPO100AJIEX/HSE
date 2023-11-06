#include <ios>
#include <iostream>
#include <string>
#include <vector>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n, a, k;
    std::cin >> n >> a >> k;
    char maxSymbol = 'a' + a;

    std::string pattern; std::cin >> pattern;
    std::vector <unsigned int> prefixArray(pattern.size());
    prefixArray[0] = 0;

    unsigned int maxFallback = 0;
    for (unsigned int i = 1; i < pattern.size(); ++i)
    {
        for (char j = 'a'; j < maxSymbol; j++)
        {
            if (pattern[i] == j) continue;
            unsigned int prefixSize = prefixArray[i - 1];
            while (prefixSize != 0 && j != pattern[prefixSize]) prefixSize = prefixArray[prefixSize - 1];
            maxFallback = std::max(maxFallback, i - prefixSize);
        }

        unsigned int prefixSize = prefixArray[i - 1];
        while (prefixSize != 0 && pattern[i] != pattern[prefixSize]) prefixSize = prefixArray[prefixSize - 1];
        prefixArray[i] = prefixSize + (pattern[i] == pattern[prefixSize]);
    }
    
    std::cout << n + maxFallback * k;
}