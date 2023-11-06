#include <ios>
#include <iostream>
#include <numeric>
#include <array>
#include <vector>
#include <string>

unsigned int levenshteinDistance(const std::string& s1, const std::string& s2)
{
    const std::string& smaller = (s1.size() < s2.size() ? s1 : s2);
    const std::string& bigger = (s1.size() < s2.size() ? s2 : s1);
    
    static std::array<unsigned int, 101> dp;
    std::iota(dp.begin(), dp.begin() + smaller.size() + 1, 0);

    for (unsigned int i = 0; i < bigger.size(); ++i)
    {
        unsigned int save = i;
        ++dp[0];
        char symbol = bigger[i];
        for (unsigned int j = 0; j < smaller.size(); ++j)
        {
            if (symbol != smaller[j]) save = std::min(std::min(dp[j], dp[j + 1]), save) + 1;
            std::swap(dp[j + 1], save);
        }
    }
    
    return dp[smaller.size()];
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int words; std::cin >> words;
    std::vector< std::string > dictionary(words);
    for (unsigned int i = 0; i < words; i++) std::cin >> dictionary[i];

    unsigned int maxDistance, queries; std::cin >> maxDistance >> queries;
    for (unsigned int i = 0; i < queries; ++i)
    {
        std::string query; std::cin >> query;
        unsigned int answer = 0;
        for (const std::string& word : dictionary)
        {
            if (levenshteinDistance(word, query) <= maxDistance) answer++;
        }
        std::cout << answer << '\n';
    }
}