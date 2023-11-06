#include <ios>
#include <iostream>
#include <utility>
#include <algorithm>
#include <vector>

constexpr unsigned int kMaxLevel = 2000000;
constexpr unsigned int kModulus = 1000000007;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector<std::pair<unsigned int, unsigned int> > answers(kMaxLevel);
    answers[0] = {0, 0};
    answers[1] = {0, 0};

    for (std::vector<std::pair<unsigned int, unsigned int> >::iterator now = answers.begin() + 2;
         now != answers.end(); ++now) {
        std::vector<std::pair<unsigned int, unsigned int> >::iterator prev1 = now - 1,
                                                                      prev2 = now - 2;
        now->first = (prev1->second + (prev2->second << 1) + 4) % kModulus;
        now->second = (prev1->first + (prev2->first << 1)) % kModulus;
        now->first = std::max(now->first, now->second);
    }

    unsigned int n;
    std::cin >> n;
    for (unsigned int i = 0; i < n; ++i) {
        unsigned int index;
        std::cin >> index;
        std::cout << answers[index - 1].first << "\n";
    }
}
