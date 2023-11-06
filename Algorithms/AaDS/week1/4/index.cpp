/*
    did some extra reading:
    https://chromium.googlesource.com/v8/v8.git/+/refs/heads/main/src/bigint/ - google's
   implementation of BigInt https://ru.algorithmica.org/cs/algebra/karatsuba/ - more theory on how
   to use karatsuba algorithm for multiplication of polynomials
*/
#include <iostream>

void multiply(uint32_t* src1, uint32_t* src2, uint32_t* dest, unsigned int size) {
    if (size <= 45) {
        for (unsigned int i = 0; i < size; i++) {
            for (unsigned int j = 0; j < size; j++) {
                dest[i + j] += src1[i] * src2[j];
            }
        }
        return;
    }
    unsigned int part_size = (size >> 1);
    uint32_t* sum1 = static_cast<uint32_t*>(malloc(part_size * sizeof(uint32_t)));
    uint32_t* sum2 = static_cast<uint32_t*>(malloc(part_size * sizeof(uint32_t)));
    uint32_t* abcd = static_cast<uint32_t*>(calloc(size, sizeof(uint32_t)));
    for (unsigned int i = 0; i < part_size; i++) {
        sum1[i] = src1[i] + src1[part_size + i];
        sum2[i] = src2[i] + src2[part_size + i];
    }
    multiply(sum1, sum2, abcd, part_size);
    free(sum1);
    free(sum2);
    multiply(src1, src2, dest, part_size);
    multiply(src1 + part_size, src2 + part_size, dest + size, part_size);
    for (unsigned int i = 0; i < part_size; i++) {
        uint32_t s1 = dest[part_size + i] + abcd[i] - dest[i] - dest[size + i];
        uint32_t s2 =
            dest[size + i] + abcd[part_size + i] - dest[part_size + i] - dest[size + part_size + i];
        dest[part_size + i] = s1;
        dest[size + i] = s2;
    }

    /*for (unsigned int i = 0; i < size; i++) {
        abcd[i] -= (dest[i] + dest[size + i]);
    }
    for (unsigned int i = 0; i < size; i++) {
        dest[part_size + i] += abcd[i];
    }*/
    free(abcd);
}

 #include <fstream>
 #include <chrono>
int main(void) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

     std::ifstream fin("input5.txt");
     std::chrono::high_resolution_clock::time_point now1 =
     std::chrono::high_resolution_clock::now();

    std::string val1, val2;
    // std::cin >> val1 >> val2;
     fin >> val1 >> val2;
    unsigned int input_size = std::max(val1.size(), val2.size()) - 1, size = 2;
    while (input_size >>= 1) {
        size <<= 1;
    }
    uint32_t* num1 = static_cast<uint32_t*>(malloc(size * sizeof(uint32_t)));
    uint32_t* num2 = static_cast<uint32_t*>(malloc(size * sizeof(uint32_t)));
    unsigned int ind = 0;
    for (std::string::reverse_iterator iter = val1.rbegin(); iter != val1.rend(); iter++) {
        num1[ind] = (*iter) - '0';
        ind++;
    }
    for (; ind < size; ind++) {
        num1[ind] = 0;
    }
    ind = 0;
    for (std::string::reverse_iterator iter = val2.rbegin(); iter != val2.rend(); iter++) {
        num2[ind] = (*iter) - '0';
        ind++;
    }
    for (; ind < size; ind++) {
        num2[ind] = 0;
    }
    unsigned int res_size = (size << 1);
    uint32_t* res = static_cast<uint32_t*>(calloc(res_size, sizeof(uint32_t)));
    multiply(num1, num2, res, size);
    for (unsigned int i = 0; i < res_size - 1; i++) {
        res[i + 1] += res[i] / 10;
        res[i] %= 10;
        if (res[i] < 0) {
            res[i] += 10;
            res[i + 1]--;
        }
    }
    ind = res_size - 1;
    while (res[ind] == 0 && ind > 0) {
        ind--;
    }
    for (int i = ind; i >= 0; i--) {
        std::cout << res[i];
    }
    free(num1);
    free(num2);
    free(res);

     std::chrono::high_resolution_clock::time_point now2 =
     std::chrono::high_resolution_clock::now(); std::cout << std::endl << std::endl; std::cout <<
     size << ": " << double((now2 - now1).count()) / 1000000 << "ms" << std::endl;
}