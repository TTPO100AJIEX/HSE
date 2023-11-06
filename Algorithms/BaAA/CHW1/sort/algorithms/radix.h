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
    std::vector<unsigned int> res(data.size()); // Create a storage for the result
    countOps(1); // create and initialize offset
    countOps(2); // access offset, compare
    for (unsigned int offset = 0; offset < 16; offset += 8) // Digits to check
    {
        countOps(256); // create and initialize 256 variables for counter
        unsigned int counter[256] = { 0 }; // Counter for counting sort by current digit

        for (const unsigned int& item : data) // Loop through input
        {
            countOps(7); // access item, access offset, binary shift, binary and, access counter[...], increment counter[...], assign counter[...]
            ++counter[(item >> offset) & 255]; // Increase the respective counter
        }
        countOps(1); // create and initialize i
        countOps(2); // access i, compare
        for (unsigned int i = 1; i < 256; ++i)
        {
            countOps(4); // access i, subtract 1, access counter[i - 1], assign
            counter[i] += counter[i - 1];
            countOps(3); // access i, increment i, assign i
            countOps(2); // access i, compare
        }
        
        countOps(2); // access rbegin, create and initialize cur
        countOps(3); // access cur, access rend, compare
        for (std::vector<unsigned int>::reverse_iterator cur = data.rbegin(); cur != data.rend(); ++cur)
        {
            countOps(9); // access cur, access *cur, access offset, binary shift, binary and, access counter[...], decrement counter[...], assign counter[...], assign res[...]
            res[--counter[(*cur >> offset) & 255]] = *cur; // Get the correct index of the current element and put it there in the resulting array
            countOps(3); // access cur, increment cur, assign cur
            countOps(3); // access cur, access rend, compare
        }
        swap(data, res); // Swap data and result to fix the state avoiding unnecessary dynamic memory allocations
        countOps(3); // access offset, increment offset, assign offset
        countOps(2); // access offset, compare
    }
}