#include <ios>
#include <iostream>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n;
    std::cin >> n;
    int* data = new int[n];
    for (unsigned int i = 0; i < n; i++) {
        std::cin >> data[i];
    }
    for (unsigned int i = 1; i < n; i++) {
        if (data[i] > data[(i - 1) >> 1]) {
            std::cout << i - 1;
            delete[] data;
            return 0;
        }
    }
    std::cout << n - 1;
    delete[] data;
}