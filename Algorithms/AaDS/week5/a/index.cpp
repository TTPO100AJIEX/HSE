#include <iostream>
#include "Tree.cpp"

int main(void)
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    Tree test;
    test.insert(92);
    test.insert(90);
    test.insert(93);
    std::cout << test.getHeight();

    return 0;
}