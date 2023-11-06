#include <ios>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector<bool> encoded;
    while (true) {
        char symbol = std::cin.get();
        if (symbol != '0' && symbol != '1') {
            break;
        }
        encoded.push_back(symbol - '0');
    }

    const unsigned int syndrome_size = static_cast<int>(log2(encoded.size())) + 1;
    std::vector<bool> syndrome(syndrome_size, 0);
    for (unsigned int i = 0; i < encoded.size(); ++i) {
        if (!encoded[i]) {
            continue;
        }
        unsigned int column = i + 1, bit_number = 0;
        while (column > 0) {
            if ((column & 1) == 1) {
                syndrome[bit_number] = !syndrome[bit_number];
            }
            column >>= 1;
            ++bit_number;
        }
    }

    unsigned error_bit = 0;
    for (unsigned int i = 0; i < syndrome_size; ++i) {
        if (syndrome[i]) {
            error_bit += (1 << i);
        }
    }
    if (error_bit != 0 && error_bit <= encoded.size()) {
        encoded[error_bit - 1] = !encoded[error_bit - 1];
    }

    for (unsigned int i = 0; i < encoded.size(); ++i) {
        if ((i & (i + 1)) != 0) {
            std::cout << encoded[i];
        }
    }
}