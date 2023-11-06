#include <ios>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n, a, k;
    std::cin >> n >> a >> k;
    char max_symbol = 'a' + a;

    std::string pattern;
    std::cin >> pattern;
    std::vector<unsigned int> prefix_array(pattern.size());
    prefix_array[0] = 0;

    unsigned int max_fallback = 0;
    for (unsigned int i = 1; i < pattern.size(); ++i) {
        for (char j = 'a'; j < max_symbol; j++) {
            if (pattern[i] == j) {
                continue;
            }
            unsigned int prefix_size = prefix_array[i - 1];
            while (prefix_size != 0 && j != pattern[prefix_size]) {
                prefix_size = prefix_array[prefix_size - 1];
            }
            max_fallback = std::max(max_fallback, i - prefix_size);
        }

        unsigned int prefix_size = prefix_array[i - 1];
        while (prefix_size != 0 && pattern[i] != pattern[prefix_size]) {
            prefix_size = prefix_array[prefix_size - 1];
        }
        prefix_array[i] = prefix_size + (pattern[i] == pattern[prefix_size]);
    }

    std::cout << n + max_fallback * k;
}