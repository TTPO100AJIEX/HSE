#include <list>
#include "Merge.cpp"
#include <iostream>
#include <chrono>

int main(void)
{
    std::list <int> A, B;
    for (int i = 0; i < 7e5; i++)
    {
        A.push_back(i); B.push_back(i);
    }
    

    std::chrono::high_resolution_clock::time_point now1 = std::chrono::high_resolution_clock::now();

    std::list <int> C = merge(A, B);

    std::chrono::high_resolution_clock::time_point now2 = std::chrono::high_resolution_clock::now();
    std::cout << A.size() << " " << ((now2 - now1).count()) / 1000000 << "ms" << std::endl;


    for (auto it = C.begin(); it != C.end(); it++)
    {
        //std::cout << *it << " ";
    }
    return 0;
}