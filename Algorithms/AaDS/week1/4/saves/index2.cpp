/*
    did some extra reading:
    https://chromium.googlesource.com/v8/v8.git/+/refs/heads/main/src/bigint/ - google's implementation of BigInt
    https://ru.algorithmica.org/cs/algebra/karatsuba/ - more theory on how to use karatsuba algorithm for multiplication of polynomials
*/

#include <iostream>
#include <string>

bool output = true;
void multiply(char* src1, char* src2, char* dest, unsigned int size)
{
    bool output_ = output;
    output = false;
    if (size == 1)
    {
        char result = src1[0] * src2[0];
        dest[0] = result % 10; dest[1] = result / 10;
        return;
    }
    if (size == 2 && src1[1] == 0 && src2[1] == 0)
    {
        char result = src1[0] * src2[0];
        dest[0] = result % 10; dest[1] = result / 10; dest[2] = dest[3] = 0;
        return;
    }
    unsigned int part_size = (size + 1) >> 1;
    char* sum1 = (char*)(alloca(part_size + 1)); char* sum2 = (char*)(alloca(part_size + 1));
    sum1[part_size] = sum2[part_size] = 0;
    if ((size & 1) == 1)
    {
        sum1[0] = src1[0] + src1[part_size];
        sum2[0] = src2[0] + src2[part_size];
        for (unsigned int i = 1; i < part_size - 1; i++)
        {
            sum1[i] = src1[i] + src1[part_size + i];
            sum2[i] = src2[i] + src2[part_size + i];
            if (sum1[i - 1] > 9) { sum1[i]++; sum1[i - 1] -= 10; }
            if (sum2[i - 1] > 9) { sum2[i]++; sum2[i - 1] -= 10; }
        }
        sum1[part_size - 1] = src1[part_size - 1];
        sum2[part_size - 1] = src2[part_size - 1];
        if (sum1[part_size - 2] > 9) { sum1[part_size - 1]++; sum1[part_size - 2] -= 10; }
        if (sum2[part_size - 2] > 9) { sum2[part_size - 1]++; sum2[part_size - 2] -= 10; }
    }
    else
    {
        sum1[0] = src1[0] + src1[part_size];
        sum2[0] = src2[0] + src2[part_size];
        for (unsigned int i = 1; i < part_size; i++)
        {
            sum1[i] = src1[i] + src1[part_size + i];
            sum2[i] = src2[i] + src2[part_size + i];
            if (sum1[i - 1] > 9) { sum1[i]++; sum1[i - 1] -= 10; }
            if (sum2[i - 1] > 9) { sum2[i]++; sum2[i - 1] -= 10; }
        }
    }
    if (sum1[part_size - 1] > 9) { sum1[part_size]++; sum1[part_size - 1] -= 10; }
    if (sum2[part_size - 1] > 9) { sum2[part_size]++; sum2[part_size - 1] -= 10; }

    multiply(src1, src2, dest, part_size);
    multiply(src1 + part_size, src2 + part_size, dest + size, part_size);

    char* abcd, sum_size = 0;
    if (sum1[part_size] == 0 && sum2[part_size] == 0)
    {
        sum_size = part_size << 1;
        abcd = (char*)(alloca(sum_size));
        multiply(sum1, sum2, abcd, part_size);
    }
    else
    {
        sum_size = (part_size + 1) << 1;
        abcd = (char*)(alloca(sum_size));
        multiply(sum1, sum2, abcd, part_size + 1);
    }
    for (unsigned int i = 0; i < size; i++)
    {
        abcd[i] -= (dest[i] + dest[size + i]);
        if (abcd[i] < 0)
        {
            abcd[i] += 10; abcd[i + 1]--;
            if (abcd[i] < 0) { abcd[i] += 10; abcd[i + 1]--; }
        }
    }
    for (int i = sum_size - 1; i >= 0; i--) std::cout << (int)(abcd[i]);
    std::cout << std::endl;
    /*if (output_)
    {
        for (int i = 2 * size - 1; i >= 0; i--) std::cout << (int)(dest[i]);
        std::cout << std::endl;
        for (int i = sum_size - 1; i >= 0; i--) std::cout << (int)(abcd[i]);
        std::cout << std::endl;
    }*/
    /*for (unsigned int i = 0; i < sum_size; i++)
    {
        dest[part_size + i] += abcd[i];
        if (dest[part_size + i] > 9) { dest[part_size + i] -= 10; dest[part_size + i + 1]++; }
    }*/


    /*for (int i = part_size; i >= 0; i--) std::cout << (int)(sum1[i]) << " ";
    std::cout << " | ";
    for (int i = part_size; i >= 0; i--) std::cout << (int)(sum2[i]) << " ";
    std::cout << " = ";*/
    //unsigned int tmp;

    //std::cout << (int)(sum1[part_size - 1]) << (int)(sum2[part_size - 1]) << std::endl;
    /*char* abcd;
    if (sum1[part_size] == 0 && sum2[part_size] == 0)
    {
        //tmp = part_size << 1;
        abcd = (char*)(alloca(part_size << 1));
        multiply(sum1, sum2, abcd, part_size);
    }
    else
    {
        //tmp = (part_size + 1) << 1;
        //std::cout << (part_size + 1) << 1 << '!';
        abcd = (char*)(alloca((part_size + 1) << 1));
        multiply(sum1, sum2, abcd, part_size + 1);
    }*/

    /*for (int i = 2 * (part_size + 1) - 1; i >= 0; i--)
    {
        std::cout << (int)(abcd[i]) << " ";
    }
    std::cout << std::endl;*/
    
    /*for (unsigned int i = 0; i < size; i++)
    {
        abcd[i] -= (dest[i] + dest[size + i]);
        if (abcd[i] < 0)
        {
            abcd[i] += 10; abcd[i + 1]--;
            if (abcd[i] < 0) { abcd[i] += 10; abcd[i + 1]--; }
        }
    }*/

    /*for (int i = 2 * part_size; i >= 0; i--)
    {
        std::cout << (int)(abcd[i]) << " ";
    }
    std::cout << std::endl;*/
    
    /*for (unsigned int i = 0; i <= size; i++)
    {
        dest[part_size + i] += abcd[i];
        if (dest[part_size + i] > 9) { dest[part_size + i] -= 10; dest[part_size + i + 1]++; }
    }*/

    /*for (unsigned int i = 0; i < part_size; i++)
    {
        std::cout << (int)(dest[part_size + i]) << " " << (int)(abcd[i]) << " " << (int)(dest[i]) << " " << (int)(dest[size + i]) << std::endl;
        std::cout << (int)(dest[size + i]) << " " << (int)(abcd[part_size + i]) << " " << (int)(dest[part_size + i]) << " " << (int)(dest[size + part_size + i]) << std::endl;
        char c1 = dest[part_size + i] + abcd[i] - dest[i] - dest[size + i];
        if (c1 < 0) { c1 += 10; abcd[i + 1]--; }
        char c2 = dest[size + i] + abcd[part_size + i] - dest[part_size + i] - dest[size + part_size + i];
        if (c2 < 0) { c2 += 10; abcd[part_size + i + 1]--; }
        dest[part_size + i] = c1;
        dest[size + i] = c2;
    }*/
}

int main(void)
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string val1, val2;
    std::cin >> val1 >> val2;
    const unsigned int size = std::max(val1.size(), val2.size());
    char* num1 = (char *)(alloca(size)); char* num2 = (char *)(alloca(size));
    for (unsigned int i = 0; i < size; i++) num1[i] = num2[i] = 0;
    unsigned int ind = 0;
    for (std::string::reverse_iterator iter = val1.rbegin(); iter != val1.rend(); iter++) { num1[ind] = (*iter) - '0'; ind++; }
    ind = 0;
    for (std::string::reverse_iterator iter = val2.rbegin(); iter != val2.rend(); iter++) { num2[ind] = (*iter) - '0'; ind++; }

    char res[size << 1] = { 0 };
    multiply(num1, num2, res, size);
    for (int i = (size << 1) - 1; i >= 0; i--) std::cout << (int)(res[i]);

    return 0;
}