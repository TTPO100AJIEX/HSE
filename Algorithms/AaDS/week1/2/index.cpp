#include <iostream>
#include "deque.cpp"

#include <chrono>
int main(void)
{
    std::chrono::high_resolution_clock::time_point now1 = std::chrono::high_resolution_clock::now();
    Deque deque;
    for (int i = 0; i < 100000; i++) deque.pushMiddle(i);
    int sum = 0;
    for (int i = 0; i < 100000; i++) sum += deque.popFront();
    std::chrono::high_resolution_clock::time_point now2 = std::chrono::high_resolution_clock::now();
    std::cout << std::endl << double((now2 - now1).count()) / 1000000 << "ms";
    std::cout << std::endl << sum << std::endl;
    return 0;
    
    /*deque.pushBack(1);
    deque.pushBack(2);
    deque.pushMiddle(3);
    std::cout << deque.popFront() << std::endl;
    deque.pushBack(4);
    deque.pushMiddle(5);
    std::cout << deque.popFront() << std::endl;
    std::cout << deque.popFront() << std::endl;
    std::cout << deque.popFront() << std::endl;
    std::cout << deque.popFront() << std::endl;*/
}