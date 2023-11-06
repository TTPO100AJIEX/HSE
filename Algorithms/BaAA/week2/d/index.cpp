#include <iostream>
#include <ios>
#include <string>

#define SYMBOLS 'Z' - 'A' + 1

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n; std::cin >> n;
    unsigned int counter[SYMBOLS] = { 0 };
    for (unsigned int i = 0; i < n; ++i) { char symbol; std::cin >> symbol; ++counter[symbol - 'A']; }
    char center_symbol = 0;
    for (unsigned int i = 0; i < SYMBOLS; ++i)
    {
        unsigned int half = (counter[i] >> 1);
        std::cout << std::string(half, (char)('A' + i));
        if ((counter[i] & 1) == 1 && center_symbol == 0) center_symbol = 'A' + i;
        counter[i] = half;
    }
    if (center_symbol != 0) std::cout << center_symbol;
    for (int i = SYMBOLS - 1; i >= 0; --i) std::cout << std::string(counter[i], (char)('A' + i));
    return 0;
}