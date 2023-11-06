#include <ios>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string text;
    std::cin >> text;
    std::vector<unsigned int> prefix_array(text.size());
    prefix_array[0] = 0;
    std::cout << 0 << ' ';
    for (unsigned int i = 1; i < text.size(); ++i) {
        unsigned int prefix_size = prefix_array[i - 1];
        while (prefix_size != 0 && text[i] != text[prefix_size]) {
            prefix_size = prefix_array[prefix_size - 1];
        }
        prefix_array[i] = prefix_size + (text[i] == text[prefix_size]);

        std::cout << prefix_array[i] << ' ';
    }
}