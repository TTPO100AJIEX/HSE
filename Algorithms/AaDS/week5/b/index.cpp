#include <iostream>
#include "Tree.cpp"

int main(void)
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    Tree test;
    test.insert(7);
    test.insert(3);
    test.insert(2);
    test.insert(1);
    test.insert(9);
    test.insert(5);
    test.insert(4);
    test.insert(6);
    test.insert(8);
    std::cout << test.max2();
    return 0;
}