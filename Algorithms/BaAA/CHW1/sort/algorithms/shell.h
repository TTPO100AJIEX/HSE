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


void sort(std::vector<unsigned int>& data)
{
    countOps(3); // access size, binary shift, create and initialize gap
    countOps(2); // access gap, compare
    for (unsigned int gap = (data.size() >> 1); gap > 0; gap >>= 1)
    {
        // GAPPED INSERTION SORT
        countOps(2); // access gap, create and initialize i
        countOps(3); // access i, access size, compare
        for (unsigned int i = gap; i < data.size(); ++i) // Loop through input
        {
            countOps(3); // access i, access data[i], create and initialize pivot
            countOps(2); // access i, create and initialize j
            unsigned int pivot = data[i], j = i;
            countOps(8); // access j, access gap, subtract, access data[j - gap], access pivot, compare x2, logical and
            for ( ; j >= gap && data[j - gap] > pivot; j -= gap) // While current element should be moved to the left (is not in order)
            {
                countOps(5); // access j, access gap, subtract, access data[j - gap], assign
                data[j] = data[j - gap]; // Move the left element one position right
                countOps(4); // access j, access gap, subtract, assign j
                countOps(8); // access j, access gap, subtract, access data[j - gap], access pivot, compare x2, logical and
            }
            countOps(2); // access pivot, assign
            data[j] = pivot; // Put current element where it should be
            countOps(3); // access i, increment i, assign i
            countOps(3); // access i, access size, compare
        }
        countOps(3); // access gap, binary shift, assign gap
        countOps(2); // access gap, compare
    }
}