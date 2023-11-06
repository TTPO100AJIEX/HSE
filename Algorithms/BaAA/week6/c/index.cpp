#include <ios>
#include <iostream>
#include <utility>
#include <algorithm>
#include <vector>

constexpr unsigned int MAX_LEVEL = 2000000;
constexpr unsigned int MODULUS = 1000000007;

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector< std::pair <unsigned int, unsigned int> > answers(MAX_LEVEL);
    answers[0] = { 0, 0 };
    answers[1] = { 0, 0 };
    
    for (unsigned int i = 2; i < MAX_LEVEL; ++i)
    {
        answers[i].first = (answers[i - 1].second + (answers[i - 2].second << 1) + 4) % MODULUS;
        answers[i].second = (answers[i - 1].first + (answers[i - 2].first << 1)) % MODULUS;
        answers[i].first = std::max(answers[i].first, answers[i].second);
    }

    unsigned int n; std::cin >> n;
    for (unsigned int i = 0; i < n; ++i)
    {
        unsigned int index; std::cin >> index;
        std::cout << answers[index - 1].first << "\n";
    }
}
