#include <iostream>
#include <vector>

int main(void) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n;
    std::cin >> n;
    std::vector<unsigned int> num_zeros(n + 1);
    num_zeros[0] = 0;
    for (unsigned int i = 0; i < n; i++) {
        int number;
        std::cin >> number;
        num_zeros[i + 1] = num_zeros[i] + (number == 0);
    }
    unsigned int k;
    std::cin >> k;
    for (unsigned int i = 0; i < k; i++) {
        unsigned int l, r;
        std::cin >> l >> r;
        std::cout << num_zeros[r] - num_zeros[l - 1] << " ";
    }

    return 0;
}