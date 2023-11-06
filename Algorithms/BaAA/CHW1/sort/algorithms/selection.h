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
    countOps(2); // access begin, create and initialize cur
    countOps(4); // access cur, access end, subtract 1, compare
    for (std::vector<unsigned int>::iterator cur = data.begin(); cur != data.end() - 1; ++cur) // Loop though input
    {
        // Find the index of the minimum item in the array suffix
        countOps(2); // access cur, create and initialize minimum
        std::vector<unsigned int>::iterator minimum = cur;
        countOps(3); // access cur, add 1, create and initialize minCur
        countOps(3); // access minCur, access end, compare
        for (std::vector<unsigned int>::iterator minCur = cur + 1; minCur != data.end(); ++minCur) // Loop through the suffix
        {
            countOps(5); // access minCur, access *minCur, access minimum, access *minimum, compare
            if (*minCur < *minimum) // If current element is lower than the previous minimum
            {
                countOps(2); // access minCur, assign
                minimum = minCur; // Update the minimum
            }
            countOps(3); // access minCur, increment minCur, assign minCur
            countOps(3); // access minCur, access end, compare
        }
        countOps(2); // access cur, access minimum
        swap(*cur, *minimum); // Put the smallest item in its correct place
        countOps(3); // access cur, increment cur, assign cur
        countOps(4); // access cur, access end, subtract 1, compare
    }
}