#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n, m;
    std::cin >> n >> m;
    std::vector<int> data(n);
    for (unsigned int i = 0; i < n; i++) {
        std::cin >> data[i];
    }

    for (unsigned int i = 0; i < m; i++) {
        int value;
        std::cin >> value;

        {
            int l = -1, r = n - 1;
            while (r - l > 1) {
                unsigned int m = ((r + l) >> 1);
                if (data[m] >= value) {
                    r = m;
                } else {
                    l = m;
                }
            }
            if (data[r] != value) {
                std::cout << 0 << "\n";
                continue;
            }
            std::cout << r + 1 << " ";
        }
        {
            int l = 0, r = n;
            while (r - l > 1) {
                unsigned int m = ((r + l) >> 1);
                if (data[m] > value) {
                    r = m;
                } else {
                    l = m;
                }
            }
            std::cout << l + 1 << "\n";
        }
    }
}