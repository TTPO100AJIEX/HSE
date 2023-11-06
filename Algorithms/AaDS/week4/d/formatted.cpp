#include <iostream>
#include <vector>
#include <algorithm>

int findDoubledMedian(int* a, int* b, unsigned int size) {
    if (size == 2) {
        return (a[0] + a[1] + b[0] + b[1] - std::min(std::min(a[0], a[1]), std::min(b[0], b[1])) -
                std::max(std::max(a[0], a[1]), std::max(b[0], b[1])));
    }
    int part_medians = (size - 1) / 2;
    if (a[part_medians] < b[part_medians]) {
        return findDoubledMedian(a + part_medians, b, size - part_medians);
    } else {
        return findDoubledMedian(a, b + part_medians, size - part_medians);
    }
}

int main(void) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n, m;
    std::cin >> n >> m;
    std::vector<std::vector<int> > data(n, std::vector<int>(m));
    for (unsigned int i = 0; i < n; i++) {
        for (unsigned int j = 0; j < m; j++) {
            std::cin >> data[i][j];
        }
    }

    for (unsigned int i = 0; i < n; i++) {
        for (unsigned int j = i + 1; j < n; j++) {
            int median = findDoubledMedian(data[i].data(), data[j].data(), m);
            if (median < 0) {
                std::cout << "-";
                median = -median;
            }
            if (median % 2 == 1) {
                std::cout << median / 2 << ".50000\n";
            } else {
                std::cout << median / 2 << ".00000\n";
            }
        }
    }

    return 0;
}