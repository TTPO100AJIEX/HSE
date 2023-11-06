/*
    did some extra reading:
    https://chromium.googlesource.com/v8/v8.git/+/refs/heads/main/src/bigint/ - google's implementation of BigInt
    https://ru.algorithmica.org/cs/algebra/karatsuba/ - more theory on how to use karatsuba algorithm for multiplication of polynomials
*/
#include <iostream>
#include <string>

void multiply(char* src1, char* src2, char* dest, unsigned int size) // size is a power of 2
{
    if (size == 1)
    {
        char result = src1[0] * src2[0];
        dest[0] = result % 10; dest[1] = result / 10;
        return;
    }
    unsigned int part_size = size >> 1;
    char* sum1 = (char*)(malloc(size)); char* sum2 = (char*)(malloc(size));
    sum1[part_size] = sum2[part_size] = 0;
    char extra1 = 0, extra2 = 0;
    for (unsigned int i = 0; i < part_size; i++)
    {
        sum1[i] = src1[i] + src1[part_size + i] + extra1;
        sum2[i] = src2[i] + src2[part_size + i] + extra2;
        extra1 = extra2 = 0;
        if (sum1[i] > 9) { sum1[i] -= 10; extra1 = 1; }
        if (sum2[i] > 9) { sum2[i] -= 10; extra2 = 1; }
    }
    sum1[part_size] = extra1; sum2[part_size] = extra2;
    char* abcd;
    unsigned int abcd_size = 0;
    if (sum1[part_size] == 0 && sum2[part_size] == 0)
    {
        abcd = (char*)(malloc(size));
        abcd_size = size;
        multiply(sum1, sum2, abcd, part_size);
    }
    else
    {
        for (unsigned int i = part_size + 1; i < size; i++) sum1[i] = sum2[i] = 0;
        abcd = (char*)(malloc(size << 1));
        abcd_size = (size << 1);
        multiply(sum1, sum2, abcd, size);
    }
    free(sum1); free(sum2);
    multiply(src1, src2, dest, part_size);
    multiply(src1 + part_size, src2 + part_size, dest + size, part_size);
    for (unsigned int i = 0; i < size; i++)
    {
        abcd[i] -= (dest[i] + dest[size + i]);
        while (abcd[i] < 0) { abcd[i] += 10; abcd[i + 1]--; }
    }
    for (unsigned int i = 0; i < std::min(abcd_size, size + 1); i++)
    {
        dest[part_size + i] += abcd[i];
        while (dest[part_size + i] > 9) { dest[part_size + i] -= 10; dest[part_size + i + 1]++; }
    }
    free(abcd);
}

//#include <fstream>
//#include <chrono>
int main(void)
{
    //std::ios::sync_with_stdio(false);
    //std::cin.tie(nullptr);

    //std::ifstream fin("input5.txt");
    //std::chrono::high_resolution_clock::time_point now1 = std::chrono::high_resolution_clock::now();

    std::string val1, val2;
    std::cin >> val1 >> val2;
    unsigned int input_size = std::max(val1.size(), val2.size()) - 1, size = 2;
    while (input_size >>= 1) size <<= 1;
    char* num1 = (char *)(malloc(size)); char* num2 = (char *)(malloc(size));
    unsigned int ind = 0;
    for (std::string::reverse_iterator iter = val1.rbegin(); iter != val1.rend(); iter++) { num1[ind] = (*iter) - '0'; ind++; }
    while (ind < size) num1[ind++] = 0;
    ind = 0;
    for (std::string::reverse_iterator iter = val2.rbegin(); iter != val2.rend(); iter++) { num2[ind] = (*iter) - '0'; ind++; }
    while (ind < size) num2[ind++] = 0;

    unsigned int res_size = (size << 1);
    char* res = (char *)(malloc(res_size));
    multiply(num1, num2, res, size);
    ind = res_size - 1;
    while (res[ind] == 0) ind--;
    for (int i = ind; i >= 0; i--) std::cout << (int)(res[i]);

    //std::chrono::high_resolution_clock::time_point now2 = std::chrono::high_resolution_clock::now();
    //std::cout << std::endl;
    //std::cout << std::endl << double((now2 - now1).count()) / 1000000 << "ms";

    free(num1);
    free(num2);
    free(res);
    return 0;
}