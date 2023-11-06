#include <iostream>
#include <cstdint>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n, m, q;
    std::cin >> n >> m >> q;
    std::vector<std::vector<int64_t> > data(n + 2, std::vector<int64_t>(m + 2, 0));
    for (unsigned int i = 1; i <= n; i++) {
        for (unsigned int j = 1; j <= m; j++) {
            std::cin >> data[i][j];
            data[i][j] += data[i - 1][j] + data[i][j - 1] - data[i - 1][j - 1];
        }
    }
    for (unsigned int i = 0; i < q; i++) {
        unsigned int x1, y1, x2, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;
        x1--;
        y1--;
        std::cout << data[x2][y2] - data[x2][y1] - data[x1][y2] + data[x1][y1] << "\n";
    }
}