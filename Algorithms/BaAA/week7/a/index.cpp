#include <ios>
#include <iostream>
#include <cstddef>
#include <string>
#include <vector>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string pattern, text; std::cin >> pattern >> text;
    if (pattern.size() == 0)
    {
        std::cout << text.size() + 1 << '\n';
        for (unsigned int i = 0; i <= text.size(); ++i) std::cout << i << '\n';
        return 0;
    }
    size_t patternSize = pattern.size(), textSize = text.size();
    pattern += '!';

    std::vector <unsigned int> patternBlocks(patternSize), patternBlocksSpecific(patternSize);
    patternBlocks[0] = patternBlocksSpecific[0] = 0;
    for (unsigned int i = 1; i < patternSize; ++i)
    {
        unsigned int prefixSize = patternBlocks[i - 1];
        while (prefixSize != 0 && pattern[i] != pattern[prefixSize]) prefixSize = patternBlocks[prefixSize - 1];
        patternBlocks[i] = prefixSize + (pattern[i] == pattern[prefixSize]);

        if (pattern[patternBlocks[i]] != pattern[i + 1]) patternBlocksSpecific[i] = patternBlocks[i];
        else patternBlocksSpecific[i] = (patternBlocks[i] == 0) ? 0 : patternBlocksSpecific[patternBlocks[i] - 1];
    }
    

    unsigned int amount = 0;
    for (unsigned int i = 0, matched = 0; i < textSize; ++i)
    {
        while (matched != 0 && pattern[matched] != text[i]) matched = patternBlocksSpecific[matched - 1];
        if (pattern[matched] == text[i]) ++matched;

        if (matched == patternSize)
        {
            matched = patternBlocksSpecific[matched - 1];
            amount++;
        }
    }
    std::cout << amount << '\n';
    for (unsigned int i = 0, matched = 0; i < textSize; ++i)
    {
        while (matched != 0 && pattern[matched] != text[i]) matched = patternBlocksSpecific[matched - 1];
        if (pattern[matched] == text[i]) ++matched;

        if (matched == patternSize)
        {
            matched = patternBlocksSpecific[matched - 1];
            std::cout << i - patternSize + 1 << '\n';
        }
    }
}
