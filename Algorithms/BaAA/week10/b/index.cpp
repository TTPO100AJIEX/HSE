#include <ios>
#include <iostream>
#include <string>
#include <vector>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    std::vector<std::string> codes(128);
    for (unsigned int symbol = 0; symbol < 128; ++symbol) codes[symbol] = std::string(1, symbol);

    unsigned int k; std::cin >> k;

    unsigned int code; std::cin >> code;
    char symbol = code; std::cout << symbol;

    for (unsigned int fragment = 1; fragment < k; ++fragment)
    {
        const std::string& oldResult = codes[code];
        std::cin >> code;

        if (code >= codes.size())
        {
            std::cout << oldResult << symbol;
            symbol = oldResult[0];
        }
        else
        {
            std::cout << codes[code];
            symbol = codes[code][0];
        }
        codes.push_back(oldResult + symbol);
    }
}