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
#include <algorithm>
#include "./utils/countOps.h"


static void sort(std::vector<unsigned int>& data, unsigned int left, unsigned int right, std::vector<unsigned int>& storage)
{
    countOps(4); // access left, access right, subtract, compare
    if (right - left == 1) return; // If the array is one element, stop

    countOps(5); // access left, access right, sum, binary shift, create and intialize breakpoint
    unsigned int breakpoint = (left + right) >> 1; // Calculate the middle point
    sort(data, left, breakpoint, storage); // Sort each part
    sort(data, breakpoint, right, storage); // Sort each part

    // MERGE
    storage.clear(); // Clear the storage. This does not change the capacity
    countOps(2); // access breakpoint, create and initialize saveBreakpoint
    countOps(2); // access left, create and initialize saveLeft
    unsigned int saveBreakpoint = breakpoint, saveLeft = left;
    countOps(7); // access left, access saveBreakpoint, access breakpoint, access right, compare x2, logical and
    while (left != saveBreakpoint && breakpoint != right) // While both parts are not fully parsed
    {
        countOps(5); // access left, access breakpoint, access data[left], access data[breakpoint], compare
        if (data[left] < data[breakpoint]) // If the next element in the left part is smaller
        {
            countOps(5); // access left, add 1, assign left, access data[left++], assign to storage[...]
            storage.push_back(data[left++]); // Take the element from the left part to the result (storage) and increment the counter
        }
        else
        {
            countOps(5); // access breakpoint, add 1, assign breakpoint, access data[breakpoint++], assign to storage[...]
            storage.push_back(data[breakpoint++]); // Take the element from the right part to the result (storage) and increment the counter
        }
        countOps(7); // access left, access saveBreakpoint, access breakpoint, access right, compare x2, logical and
    }
    countOps(3); // access left, access saveBreakpoint, compare
    while (left != saveBreakpoint) // While (if) left part is not fully parsed
    {
        countOps(5); // access left, add 1, assign left, access data[left++], assign to storage[...]
        storage.push_back(data[left++]); // Take the next element from the left part
        countOps(3); // access left, access saveBreakpoint, compare
    }
    countOps(3); // access breakpoint, access right, compare
    while (breakpoint != right) // While (if) right part is not fully parsed
    {
        countOps(5); // access breakpoint, add 1, assign breakpoint, access data[breakpoint++], assign to storage[...]
        storage.push_back(data[breakpoint++]); // Take the next element from the right part
        countOps(3); // access breakpoint, access right, compare
    }
    countOps(5); // access storage.begin, access storage.end, access data.begin, access saveLeft, sum
    countOps(2 * storage.size()); // In the RAM model copying is just "access + assign" storage.size() times.
    std::copy(storage.begin(), storage.end(), data.begin() + saveLeft); // Copy the items from storage to the initial array
}

void sort(std::vector<unsigned int>& data)
{
    std::vector <unsigned int> storage; // Storage for the merged array to get rid of cyclic dynamic memory allocation
    storage.reserve(data.size()); // Reserve some size in storage for better performance
    sort(data, 0, data.size(), storage); // Call the helper
}