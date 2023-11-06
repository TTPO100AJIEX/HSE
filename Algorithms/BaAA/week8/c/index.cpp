#include <ios>
#include <iostream>
#include <vector>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector<bool> encoded(1, false);
    while (true)
    {
        char symbol = std::cin.get();
        if (symbol != '0' && symbol != '1') break;
        if ((encoded.size() & (encoded.size() + 1)) == 0) encoded.push_back(false);
        encoded.push_back(symbol - '0');

        if (!encoded.back()) continue;
        unsigned int column = encoded.size(), bitShift = 1;
        while (column > 0)
        {
            if ((column & 1) == 1) encoded[bitShift - 1] = !encoded[bitShift - 1];
            column >>= 1;
            bitShift <<= 1;
        }
    }

    for (bool bit : encoded) std::cout << bit;
}