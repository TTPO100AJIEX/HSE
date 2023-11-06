#include <ios>
#include <iostream>
#include <utility>
#include <vector>
#include <string>

bool accepts(const std::string& s1, const std::string& s2, unsigned int maxErrors)
{
    const std::string& smaller = (s1.size() < s2.size() ? s1 : s2);
    const std::string& bigger = (s1.size() < s2.size() ? s2 : s1);

    static std::vector< std::pair <unsigned int, unsigned int> > prev, cur;
    prev.clear(); prev.emplace_back(0, 0);
    for (char symbol : bigger)
    {
        cur.clear();
        for (unsigned int i = 0; i < prev.size(); ++i)
        {
            std::pair <unsigned int, unsigned int> state = prev[i];
            if (state.second > maxErrors || state.first > smaller.size()) continue;;
            if (smaller[state.first] == symbol)
            {
                cur.emplace_back(state.first + 1, state.second); // Correct symbol
            }
            else
            {
                cur.emplace_back(state.first, state.second + 1); // Insert into smaller
                cur.emplace_back(state.first + 1, state.second + 1); // Change in smaller
                prev.emplace_back(state.first + 1, state.second + 1); // Delete from smaller
            }
        }
        std::swap(prev, cur);
    }
    for (const std::pair <unsigned int, unsigned int>& state : prev)
    {
        if (state.first <= smaller.size() && state.second + smaller.size() - state.first <= maxErrors) return true;
    }
    return false;
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
        for (const std::string& word : dictionary) answer += accepts(word, query, maxDistance);
        std::cout << answer << '\n';
    }
}