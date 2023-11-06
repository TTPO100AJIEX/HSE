#include <ios>
#include <iostream>
#include <cstddef>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string pattern, text;
    std::cin >> pattern >> text;
    if (pattern.empty()) {
        std::cout << text.size() + 1 << '\n';
        for (unsigned int i = 0; i <= text.size(); ++i) {
            std::cout << i << '\n';
        }
        return 0;
    }
    size_t pattern_size = pattern.size(), text_size = text.size();
    pattern += '!';

    std::vector<unsigned int> pattern_blocks(pattern_size), pattern_blocks_specific(pattern_size);
    pattern_blocks[0] = pattern_blocks_specific[0] = 0;
    for (unsigned int i = 1; i < pattern_size; ++i) {
        unsigned int prefix_size = pattern_blocks[i - 1];
        while (prefix_size != 0 && pattern[i] != pattern[prefix_size]) {
            prefix_size = pattern_blocks[prefix_size - 1];
        }
        pattern_blocks[i] = prefix_size + (pattern[i] == pattern[prefix_size]);

        if (pattern[pattern_blocks[i]] != pattern[i + 1]) {
            pattern_blocks_specific[i] = pattern_blocks[i];
        } else {
            pattern_blocks_specific[i] =
                (pattern_blocks[i] == 0) ? 0 : pattern_blocks_specific[pattern_blocks[i] - 1];
        }
    }

    unsigned int amount = 0;
    for (unsigned int i = 0, matched = 0; i < text_size; ++i) {
        while (matched != 0 && pattern[matched] != text[i]) {
            matched = pattern_blocks_specific[matched - 1];
        }
        if (pattern[matched] == text[i]) {
            ++matched;
        }

        if (matched == pattern_size) {
            matched = pattern_blocks_specific[matched - 1];
            amount++;
        }
    }
    std::cout << amount << '\n';
    for (unsigned int i = 0, matched = 0; i < text_size; ++i) {
        while (matched != 0 && pattern[matched] != text[i]) {
            matched = pattern_blocks_specific[matched - 1];
        }
        if (pattern[matched] == text[i]) {
            ++matched;
        }

        if (matched == pattern_size) {
            matched = pattern_blocks_specific[matched - 1];
            std::cout << i - pattern_size + 1 << '\n';
        }
    }
}
