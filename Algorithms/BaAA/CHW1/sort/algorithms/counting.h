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
    countOps(1); // create and initialize max
    // Find the maximum element in the array
    unsigned int max = 0;
    for (const unsigned int& item : data) // Loop through the array
    {
        countOps(3); // access item, access max, compare
        if (item > max) // If current element is bigger than the previous maximum
        {
            countOps(2); // access item, assignment
            max = item; // Update the maximum
        }
    }
    countOps(2); // access max, add 1
    countOps(max + 1); // create and initialize (max + 1) variables for counter
    countOps(1); // access size for res
    std::vector<unsigned int> counter(max + 1, 0), res(data.size()); // Create the counter array and an array to save the result into
    for (const unsigned int& item : data) // Loop though items in the array
    {
        countOps(4); // access item, access counter[item], add 1, assign counter[item]
        ++counter[item]; // Increment the respective counter
    }
    countOps(3); // access begin, add 1, create and initialize it
    countOps(3); // access it, access end, compare
    for (std::vector<unsigned int>::iterator it = counter.begin() + 1; it != counter.end(); ++it)
    {
        countOps(6); // access it, access *it, subtract 1, access *(it - 1), sum, assign
        *it += *(it - 1);
        countOps(3); // access it, increment it, assign it
        countOps(3); // access it, access end, compare
    }
    countOps(2); // access rbegin, create and initialize cur
    countOps(3); // access cur, access rend, compare
    for (std::vector<unsigned int>::reverse_iterator cur = data.rbegin(); cur != data.rend(); ++cur) // Loop through data
    {
        countOps(6); // access cur, access *cur, access counter[*cur], subtract 1, assign, assign res[...]
        res[--counter[*cur]] = *cur; // Get the correct index of the current element and put it there in the resulting array
        countOps(3); // access cur, increment cur, assign cur
        countOps(3); // access cur, access rend, compare
    }
    countOps(1); // assign
    data = res; // Assign the sorted array to the initial array
}