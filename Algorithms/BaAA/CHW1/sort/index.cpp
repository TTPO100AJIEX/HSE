/*
1) АиСД-2, 2023, задание 5
2) Абрамов Александр Сергеевич БПИ213
3) Visual Studio Code
4) Реализованы 13 алгоритмов сортировки массива, программы генерации тестовых данных и проведения тестирования,
    а также измерения времени выполнения и количества произведённых элементарных операций.
    Реализованы программы обработки полученных данных: формирования корректных xlsx-файлов и построения графиков.
5) Если я правильно понял задание, то все сделано)
*/
#include <iostream>
#include <fstream>
#include <vector>
#ifdef COUNT_TIME
    #include <chrono>
#endif

// Choose the needed sorting algorithm
#if defined(BINARY_INSERTION)
    #include "./algorithms/binary-insertion.h"
#elif defined(BUBBLE_1)
    #include "./algorithms/bubble-1.h"
#elif defined(BUBBLE_2)
    #include "./algorithms/bubble-2.h"
#elif defined(BUBBLE)
    #include "./algorithms/bubble.h"
#elif defined(COUNTING)
    #include "./algorithms/counting.h"
#elif defined(HEAP)
    #include "./algorithms/heap.h"
#elif defined(INSERTION)
    #include "./algorithms/insertion.h"
#elif defined(MERGE)
    #include "./algorithms/merge.h"
#elif defined(QUICK)
    #include "./algorithms/quick.h"
#elif defined(RADIX)
    #include "./algorithms/radix.h"
#elif defined(SELECTION)
    #include "./algorithms/selection.h"
#elif defined(SHELL_CIUR)
    #include "./algorithms/shell-ciur.h"
#elif defined(SHELL)
    #include "./algorithms/shell.h"
#else
    #error "One of sorting algorithms must be defined"
#endif

// Initialize the operations counter if needed
#ifdef COUNT_ELEMENTARY_OPERATIONS
    unsigned long long int elementaryOperations = 0;
    #include "./algorithms/utils/countOps.h"
#endif

int main(int argc, char** argv)
{
    std::ifstream fin(argv[1]);
    unsigned int size; fin >> size; // Read array size
    std::vector <unsigned int> data(size);
    for (unsigned int i = 0; i < size; i++) fin >> data[i]; // Read array

	#ifdef COUNT_TIME
        // Choose the best clock
        using clock = std::conditional_t<std::chrono::high_resolution_clock::is_steady, std::chrono::high_resolution_clock, std::chrono::steady_clock>;
        clock::time_point start = clock::now(); // Get the starting timestamp
    #endif
    sort(data); // Sort the array
	#ifdef COUNT_TIME
        clock::time_point end = clock::now(); // Get the ending timestamp
    #endif

    // Print the array if needed
    #if defined(DEBUG) || defined(TEST)
        for (unsigned int i = 0; i < size; i++)
        {
            std::cout << data[i];
            if (i != size - 1) std::cout << " ";
        }
    #endif

    // Print the time spent if needed
	#ifdef COUNT_TIME
        std::cout << "Time spent: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << "ns";
    #endif
    // Print the amount of elementary operations done if needed
    #ifdef COUNT_ELEMENTARY_OPERATIONS
        std::cout << "Operations done: " << elementaryOperations;
    #endif
    
    return 0;
}