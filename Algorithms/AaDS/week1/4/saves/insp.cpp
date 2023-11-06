/*
    did some extra reading:
    https://chromium.googlesource.com/v8/v8.git/+/refs/heads/main/src/bigint/ - google's implementation of BigInt
    https://ru.algorithmica.org/cs/algebra/karatsuba/ - more theory on how to use karatsuba algorithm for multiplication of polynomials
*/
#include <iostream>
#include <vector>
#include <cstdint>

void karatsuba(char* a, char* b, char* dest, int size)
{
    if (size <= 1)
    {
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                dest[i + j] += a[i] * b[j];
        return;
    }
    int k = size / 2;
    char *l = (char *)(alloca(k)), *r = (char *)(alloca(k)), *t = (char *)(calloc(size, sizeof(char)));
    for (int i = 0; i < k; i++) {
        l[i] = a[i] + a[k + i];
        r[i] = b[i] + b[k + i];
    }
    karatsuba(l, r, t, k); // считает t
    karatsuba(a, b, dest, k); // считает p1
    karatsuba(a + k, b + k, dest + size, k); // считает p2
    char *t1 = t, *t2 = t + k;
    char *s1 = dest, *s2 = dest + k, *s3 = dest + 2 * k, *s4 = dest + 3 * k;
    for (int i = 0; i < k; i++) {
        char c1 = s2[i] + t1[i] - s1[i] - s3[i];
        char c2 = s3[i] + t2[i] - s2[i] - s4[i];
        dest[k + i] = c1;
        dest[size + i] = c2;
    }
}

#include <chrono>
#include <fstream>
int main(void) {

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    const int t = 100000;
    char a[t], b[t];
    for (int i = 0; i < t; i++) { a[i] = 1; b[i] = 2; }

    /*for (int i = 0; i < t; i++) std::cout << (int)(a[i]) << " ";
    std::cout << std::endl;
    for (int i = 0; i < t; i++) std::cout << (int)(b[i]) << " ";*/

    std::chrono::high_resolution_clock::time_point now1 = std::chrono::high_resolution_clock::now();
    char c[2 * t] = { 0 };
    karatsuba(a, b, c, t);
    std::chrono::high_resolution_clock::time_point now2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 2 * t; i++) std::cout << (int)(c[i]) << " ";
    std::cout << std::endl;
    std::cout << std::endl << double((now2 - now1).count()) / 1000000 << "ms";


    /*std::ifstream fin("input.txt");
    BigInt a, b;
    fin >> a >> b;

    size_t t1 = a.length(), t2 = b.length();
    std::chrono::high_resolution_clock::time_point now1 = std::chrono::high_resolution_clock::now();

    a *= b;

    std::chrono::high_resolution_clock::time_point now2 = std::chrono::high_resolution_clock::now();

    std::cout << a;

    std::cout << std::endl << std::endl << t1 << " " << t2 << std::endl;
    std::cout << std::endl << double((now2 - now1).count()) / 1000000 << "ms";*/
    return 0;
}