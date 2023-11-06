#include <iostream>
#include <iomanip>
#include <ios>
#include <cmath>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    double c;
    std::cin >> c;
    double l = 0, r = 1e5;
    while (r - l > 1e-7) {
        double x = (r + l) / 2, value = x * x + sqrt(x);
        if (value > c) {
            r = x;
        } else {
            l = x;
        }
    }
    std::cout << std::setprecision(7) << std::fixed << l;
}