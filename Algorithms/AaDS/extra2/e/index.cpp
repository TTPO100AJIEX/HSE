#include "./BloomFilter.cpp"
#include <iostream>

int main()
{
    BloomFilter filter(2, 10);
    filter.add("");

    std::cout << filter.verify("") << std::endl;
    std::cout << filter.verify("test2") << std::endl;

    std::cout << std::endl;

    std::cout << filter.getFPRate();
    return 0;
}