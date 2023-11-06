#include <ios>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <algorithm>

unsigned int levenshteinDistance(const std::string& s1, const std::string& s2) {
    const std::string& smaller = (s1.size() < s2.size() ? s1 : s2);
    const std::string& bigger = (s1.size() < s2.size() ? s2 : s1);

    std::vector<unsigned int> dp(smaller.size() + 1);
    std::iota(dp.begin(), dp.end(), 0);

    for (unsigned int i = 0; i < bigger.size(); ++i) {
        unsigned int save = i;
        ++dp[0];
        char symbol = bigger[i];
        for (unsigned int j = 0; j < smaller.size(); ++j) {
            if (symbol != smaller[j]) {
                save = std::min(std::min(dp[j], dp[j + 1]), save) + 1;
            }
            std::swap(dp[j + 1], save);
        }
    }

    return dp[smaller.size()];
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string s1, s2;
    std::getline(std::cin, s1);
    std::getline(std::cin, s2);
    std::cout << levenshteinDistance(s1, s2);
}