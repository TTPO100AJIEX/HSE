#include <iostream>

uint64_t findMaxPrimeDivisor(uint64_t number) {
    for (uint64_t i = 2; i * i <= number; i++) {
        if (number % i == 0) {
            number /= i--;
        }
    }
    return (number);
}

int main(void) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    uint64_t a, b;
    std::cin >> a >> b;
    uint64_t gcd = a, gcd_tmp = b;
    while (gcd != 0 && gcd_tmp != 0) {
        if (gcd > gcd_tmp) {
            gcd %= gcd_tmp;
        } else {
            gcd_tmp %= gcd;
        }
    }
    gcd += gcd_tmp;
    a /= gcd;
    b /= gcd;
    std::cout << gcd * std::max(findMaxPrimeDivisor(a), findMaxPrimeDivisor(b));
    return 0;
}