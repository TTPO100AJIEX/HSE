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


static void sort(std::vector<unsigned int>& data, unsigned int left, unsigned int right)
{
    countOps(4); // access left, access right, sum, compare
    if (right < left + 2) return; // If the array is one element, stop

    // PARITION
    countOps(3); // access left, access data[left], create and initialize pivot
    countOps(3); // access right, subtract 1, create and initialize firstLower
    unsigned int pivot = data[left], firstLower = right - 1; // Save pivot and first found element lower than the pivot
    countOps(2); // access firstLower, create and initialize i
    countOps(3); // access i, access left, compare
    for (unsigned int i = firstLower; i > left; --i) // Loop through the array
    {
        countOps(4); // access i, access data[i], access pivot, compare
        if (data[i] <= pivot) continue; // If the item is greater than pivot, it is in the correct part
        countOps(2); // access i, access firstLower
        swap(data[i], data[firstLower]); // Swap the current element with the found element to put it to the left of the pivot
        countOps(3); // access firstLower, decrement firstLower, assign firstLower
        --firstLower; // Decrease the pointer
        countOps(3); // access i, decrement i, assign i
        countOps(3); // access i, access left, compare
    }
    countOps(2); // access firstLower, access left
    swap(data[firstLower], data[left]); // Put the pivot in its correct place
    sort(data, left, firstLower); // Sort the left part
    countOps(2); // access firstLower, add
    sort(data, firstLower + 1, right); // Sort the right part
}

void sort(std::vector<unsigned int>& data)
{
    sort(data, 0, data.size()); // Call the helper
}