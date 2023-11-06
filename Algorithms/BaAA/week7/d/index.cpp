#include <ios>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <algorithm>

unsigned int DamerauLevenshteinDistance(const std::string& s1, const std::string& s2)
{
    const std::string& smaller = (s1.size() < s2.size() ? s1 : s2);
    const std::string& bigger = (s1.size() < s2.size() ? s2 : s1);
    
    static std::vector<unsigned int> prev2(4001), prev(4001), cur(4001);
    std::fill(prev2.begin(), prev2.begin() + smaller.size(), 0);
    std::iota(prev.begin(), prev.begin() + smaller.size(), 0);

    for (unsigned int i = 0; i < bigger.size(); ++i)
    {
        cur[0] = i + 1;
        for (unsigned int j = 0; j < smaller.size(); ++j)
        {
            unsigned int ans;
            if (bigger[i] == smaller[j]) ans = prev[j];
            else ans = std::min(std::min(prev[j], prev[j + 1]), cur[j]) + 1;
            if (i != 0 && j != 0 && bigger[i] == smaller[j - 1] && bigger[i - 1] == smaller[j]) ans = std::min(ans, prev2[j - 1] + (bigger[i] != smaller[j]));
            cur[j + 1] = ans;
        }
        std::swap(prev2, prev);
        std::swap(prev, cur);
    }

    return prev[smaller.size()];
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n; std::cin >> n;
    std::string s1, s2; std::getline(std::cin, s1);
    for (unsigned int i = 0; i < n; ++i)
    {
        std::getline(std::cin, s1); std::getline(std::cin, s2);
        std::cout << DamerauLevenshteinDistance(s1, s2) << ' ';
    }
}