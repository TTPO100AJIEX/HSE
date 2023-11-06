#include <ios>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector<bool> encoded;
    while (true)
    {
        char symbol = std::cin.get();
        if (symbol != '0' && symbol != '1') break;
        encoded.push_back(symbol - '0');
    }
    
    const unsigned int syndromeSize = (int)(log2(encoded.size())) + 1;
    std::vector<bool> syndrome(syndromeSize, 0);
    for (unsigned int i = 0; i < encoded.size(); ++i)
    {
        if (!encoded[i]) continue;
        unsigned int column = i + 1, bitNumber = 0;
        while (column > 0)
        {
            if ((column & 1) == 1) syndrome[bitNumber] = !syndrome[bitNumber];
            column >>= 1;
            ++bitNumber;
        }
    }

    unsigned errorBit = 0;
    for (unsigned int i = 0; i < syndromeSize; ++i)
    {
        if (syndrome[i]) errorBit += (1 << i);
    }
    if (errorBit != 0 && errorBit <= encoded.size()) encoded[errorBit - 1] = !encoded[errorBit - 1];
    
    for (unsigned int i = 0; i < encoded.size(); ++i)
    {
        if ((i & (i + 1)) != 0) std::cout << encoded[i];
    }
}