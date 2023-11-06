#include <iostream>
#include "Heap.cpp"

int main()
{
    std::vector <int> a = { 1, 2, 3, 4 };

    Heap<int> test{ 5, 3, 6, 1, 8, 0, 7 };
    Heap<int> test2 = std::move(test);
    Heap<int> test3 = test2;
    std::cout << test.size() << " " << test2.size() << " " << test3.size() << std::endl;
    test.insert(9);

    while (test.size() != 0)
    {
        std::cout << test.extract() << " ";
    }
    std::cout << std::endl;
    while (test2.size() != 0)
    {
        std::cout << test2.extract() << " ";
    }
    std::cout << std::endl;
    while (test3.size() != 0)
    {
        std::cout << test3.extract() << " ";
    }
}