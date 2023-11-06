#include <iostream>
#include <inttypes.h>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    uint16_t max = 0, max2 = 0, cur;
    std::cin >> cur;
    do {
        if (cur >= max) {
            max2 = max;
            max = cur;
        } else if (cur >= max2) {
            max2 = cur;
        }
        std::cin >> cur;
    } while (cur != 0);
    std::cout << max << "\n" << max2;
}