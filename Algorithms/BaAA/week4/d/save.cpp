#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>

typedef std::pair< std::pair<unsigned int, unsigned int>, unsigned int > VectorType;
typedef std::vector<VectorType> Vector;

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n; std::cin >> n;
    std::vector< std::vector<char> > field(n + 4, std::vector<char>(n + 4, '.'));
    for (unsigned int i = 2; i < n + 2; i++)
    {
        for (unsigned int j = 2; j < n + 2; j++)
        {
            std::cin >> field[i][j];
        }
    }
    while (true)
    {
        Vector intersections;
        for (unsigned int i = 2; i < n + 2; i++)
        {
            for (unsigned int j = 2; j < n + 2; j++)
            {
                if (field[i][j] != 'X') continue;
                unsigned int cou = 0;

                if (field[i][j - 2] == 'X' && field[i][j - 1] == 'X') cou++;
                if (field[i][j - 1] == 'X' && field[i][j + 1] == 'X') cou++;
                if (field[i][j + 2] == 'X' && field[i][j + 1] == 'X') cou++;
                
                if (field[i - 2][j] == 'X' && field[i - 1][j] == 'X') cou++;
                if (field[i - 1][j] == 'X' && field[i + 1][j] == 'X') cou++;
                if (field[i + 2][j] == 'X' && field[i + 1][j] == 'X') cou++;

                if (cou != 0) intersections.push_back({ { i, j }, cou });
            }
        }
        if (intersections.size() == 0) break;
        std::sort(intersections.begin(), intersections.end(), [](const VectorType& first, const VectorType& second) { return first.second > second.second; });
        const unsigned int i = intersections[0].first.first, j = intersections[0].first.second;
        field[i][j] = '0';
    }
    for (unsigned int i = 2; i < n + 2; i++)
    {
        for (unsigned int j = 2; j < n + 2; j++)
        {
            std::cout << field[i][j];
        }
        std::cout << '\n';
    }
}