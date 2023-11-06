#include <iostream>
#include <vector>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n; std::cin >> n;
    std::vector< std::vector<char> > field(n, std::vector<char>(n, '.'));
    for (unsigned int i = 0; i < n; ++i)
    {
        for (unsigned int j = 0; j < n; ++j) std::cin >> field[i][j];
    }

    unsigned int bestOffset = 0, bestResult = (1 << 31);
    for (unsigned int offset = 0; offset < 3; ++offset)
    {
        unsigned int result = 0;
        for (unsigned int i = 0; i < n; ++i)
        {
            for (unsigned int j = (i + offset) % 3; j < n; j += 3) result += (field[i][j] == 'X');
        }
        if (result < bestResult) { bestResult = result; bestOffset = offset; }
    }
    for (unsigned int i = 0; i < n; ++i)
    {
        for (unsigned int j = (i + bestOffset) % 3; j < n; j += 3)
        {
            if (field[i][j] == 'X') field[i][j] = '0';
        }
    }

    for (unsigned int i = 0; i < n; ++i)
    {
        for (unsigned int j = 0; j < n; ++j) std::cout << field[i][j];
        std::cout << '\n';
    }
}