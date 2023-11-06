/*
1) АиСД-2, 2023, задание 5
2) Абрамов Александр Сергеевич БПИ213
3) Visual Studio Code
4) Реализованы 13 алгоритмов сортировки массива, программы генерации тестовых данных и проведения тестирования,
    а также измерения времени выполнения и количества произведённых элементарных операций.
    Реализованы программы обработки полученных данных: формирования корректных xlsx-файлов и построения графиков.
5) Если я правильно понял задание, то все сделано)
*/
#pragma once

#include <vector>
#include "./utils/countOps.h"
#include "./utils/swap.h"


void sort(std::vector<unsigned int>& data)
{
    countOps(1); // create and initialize i
    countOps(3); // access size, subtract, create and initialize lastIndex
    countOps(7); // access lastIndex, compare lastIndex, access i, access size, substract, compare, logical and
    for (unsigned int i = 0, lastIndex = data.size() - 1; lastIndex != 0 && i < data.size() - 1; ++i) // Loop through array saving the index of the last unsorted item
    {
        countOps(2); // access lastIndex, create and initialize savedLastIndex
        unsigned int savedLastIndex = lastIndex;
        countOps(1); // assignment
        lastIndex = 0; // If there will be no swaps, the loop should stop
        countOps(1); // create and initialize j
        countOps(3); // access j, access savedLastIndex, compare
        for (unsigned int j = 0; j < savedLastIndex; ++j) // Loop through the unsorted part of array
        {
            countOps(5); // access j, add 1, access data[j], access data[j + 1], comparison
            if (data[j] > data[j + 1]) // If the elements are not in order
            {
                countOps(2); // access j, add 1
                swap(data[j], data[j + 1]); // Swap the elements to put them in the correct order
                countOps(2); // access j, assignment
                lastIndex = j; // If there will be no swaps after this, the entire suffix is already sorted
            }
            countOps(3); // access j, increment j, assign j
            countOps(3); // access j, access savedLastIndex, compare
        }
        countOps(3); // access i, increment i, assign i
        countOps(7); // access lastIndex, compare lastIndex, access i, access size, substract, compare, logical and
    }
}