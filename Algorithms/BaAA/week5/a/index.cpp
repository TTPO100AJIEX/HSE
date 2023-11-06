#include <ios>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string firstString; std::cin >> firstString;
    std::vector <int> prev(firstString.size() + 1, 0), cur(firstString.size() + 1, 0);

    char secondStringSymbol;
    while (std::cin >> secondStringSymbol)
    {
        for (unsigned int j = 0; j < firstString.size(); ++j)
        {
            if (secondStringSymbol == firstString[j]) cur[j + 1] = prev[j] + 1;
            else cur[j + 1] = std::max(prev[j + 1], cur[j]);
        }
        std::swap(prev, cur);
    }

    std::cout << prev[firstString.size()];
    return 0;
}