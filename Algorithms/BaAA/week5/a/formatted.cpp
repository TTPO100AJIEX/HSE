#include <ios>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string first_string;
    std::cin >> first_string;
    std::vector<int> prev(first_string.size() + 1, 0), cur(first_string.size() + 1, 0);

    char second_string_symbol;
    while (std::cin >> second_string_symbol) {
        for (unsigned int j = 0; j < first_string.size(); ++j) {
            if (second_string_symbol == first_string[j]) {
                cur[j + 1] = prev[j] + 1;
            } else {
                cur[j + 1] = std::max(prev[j + 1], cur[j]);
            }
        }
        std::swap(prev, cur);
    }

    std::cout << prev[first_string.size()];
    return 0;
}