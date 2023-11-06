#include "Vector.cpp"
#include <iostream>

int main(void)
{
    int* tmp = (int*)(calloc(5, sizeof(int)));
    for (int j = 0; j < 5; j++) tmp[j] = j;

    Vector test3({ 1, 7, 3, 2, 9, 0, 15, -11, 7 });
    for (unsigned int i = 0; i < test3.getSize(); i++)
    {
        std::cout << test3[i] << " ";
    }
    std::cout << std::endl;
    insertionSort(test3.begin(), test3.end());
    for (unsigned int i = 0; i < test3.getSize(); i++)
    {
        std::cout << test3[i] << " ";
    }
}