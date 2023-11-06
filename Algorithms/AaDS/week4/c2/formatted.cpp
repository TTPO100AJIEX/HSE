#include <iostream>
#include <cmath>

int main(void) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    if (n == 0) {
        std::cout << 0;
        return 0;
    }

    for (int i = sqrt(n); i > 0; i--) {
        for (int j = 0; j <= i && i * i + j * j <= n; j++) {
            for (int k = 0; k <= j && i * i + j * j + k * k <= n; k++) {
                int t = sqrt(n - (i * i + j * j + k * k));
                if (i * i + j * j + k * k + t * t == n) {
                    std::cout << i << " " << j << " " << k << " " << t;
                    return 0;
                }
            }
        }
    }

    return 0;
}