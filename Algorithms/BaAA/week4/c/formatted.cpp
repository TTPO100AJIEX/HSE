#include <iostream>
#include <cstdint>
#include <vector>

struct State {
    unsigned int left;
    unsigned int right;
    int64_t sum;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n;
    std::cin >> n;
    State best = {0, 0, static_cast<int64_t>(-1e18)};
    for (State current = {0, 0, 0}; current.right < n; ++current.right) {
        int number;
        std::cin >> number;
        current.sum += number;
        if (current.sum < 0) {
            current.left = current.right + 1;
            current.sum = 0;
            if (number > best.sum) {
                best = {current.right, current.right, number};
            }
            continue;
        }
        if (current.sum > best.sum) {
            best = current;
        }
    }
    std::cout << best.left << " " << best.right << std::endl;
}