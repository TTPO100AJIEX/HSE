#include <ios>
#include <iostream>
#include <string>
#include <vector>
#include <map>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cin >> std::noskipws;

    std::map<std::string, unsigned int> codes;
    for (unsigned int symbol = 0; symbol < 128; ++symbol) {
        codes.insert_or_assign(std::string(1, symbol), codes.size());
    }

    char symbol = 32;
    std::string current;
    std::vector<unsigned int> answer;
    while (symbol >= 32 && symbol <= 126) {
        std::cin >> symbol;
        if (codes.find(current + symbol) != codes.end()) {
            current += symbol;
        } else {
            answer.push_back(codes.find(current)->second);
            codes.insert_or_assign(current + symbol, codes.size());
            current = std::string(1, symbol);
        }
    }

    std::cout << answer.size() << '\n';
    for (unsigned int ans : answer) {
        std::cout << ans << ' ';
    }
}