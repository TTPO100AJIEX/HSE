#include <iostream>
#include "queue.cpp"

int main(void)
{
    {
        Queue queue;
        queue.push(1);
        queue.push(2);
        std::cout << queue.min() << " ";
        std::cout << queue.pop() << std::endl;
        queue.push(3);
        queue.push(4);
        std::cout << queue.min() << " ";
        std::cout << queue.pop() << std::endl;
        std::cout << queue.min() << " ";
        std::cout << queue.pop() << std::endl;
    }
    std::cout << "\n\n\n";
    {
        Queue queue;
        queue.push(2);
        queue.push(3);
        queue.push(4);
        std::cout << queue.min() << " ";
        std::cout << queue.pop() << std::endl;
        queue.push(1);
        queue.push(5);
        std::cout << queue.min() << " ";
        std::cout << queue.pop() << std::endl;
        std::cout << queue.min() << " ";
        std::cout << queue.pop() << std::endl;
        std::cout << queue.min() << " ";
        std::cout << queue.pop() << std::endl;
        std::cout << queue.min() << " ";
        std::cout << queue.pop() << std::endl;
    }
    return 0;
}