#include <ios>
#include <iostream>
#include <vector>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string text; std::cin >> text;
    std::vector <unsigned int> prefixArray(text.size());
    prefixArray[0] = 0;
    for (unsigned int i = 1; i < text.size(); ++i)
    {
        unsigned int prefixSize = prefixArray[i - 1];
        while (prefixSize != 0 && text[i] != text[prefixSize]) prefixSize = prefixArray[prefixSize - 1];
        prefixArray[i] = prefixSize + (text[i] == text[prefixSize]);
    }

    std::cout << text.size() - prefixArray[text.size() - 1];
}