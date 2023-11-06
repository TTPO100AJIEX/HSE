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


static void bubbleDown(std::vector<unsigned int>& data, unsigned int index, unsigned int size)
{
    countOps(4); // access index, binary shift, add 1, create and initialize leftIndex
    countOps(4); // access index, binary shift, add 2, create and initialize rightIndex
    const unsigned int leftIndex = (index << 1) + 1, rightIndex = (index << 1) + 2; // Calculate indexes of left and right children
    countOps(2); // access size, create and initialize swapWith
    unsigned int swapWith = size;
    countOps(8); // access leftIndex, access size, access index, access data[index], access data[leftIndex], compare x2, logical and
    if (leftIndex < size && data[index] < data[leftIndex]) // If left child exists and is smaller than current
    {
        countOps(2); // access leftIndex, assign
        swapWith = leftIndex; // The current item should be swapped with the left child
    }
    countOps(12); // access rightIndex, access size, access index, access leftIndex, access data[index], access data[rightIndex], access data[leftIndex], compare x3, logical and x2
    if (rightIndex < size && data[index] < data[rightIndex] && data[leftIndex] < data[rightIndex]) // If right child exists and is smaller than both current and left
    {
        countOps(2); // access rightIndex, assign
        swapWith = rightIndex; // The current item should be swapped with the right child
    }
    countOps(3); // access swapWith, access size, compare
    if (swapWith == size) return; // If the value of swapWith has not been changed, the heap is fixed
    countOps(2); // access index, access swapWith
    swap(data[index], data[swapWith]); // Swap the items
    bubbleDown(data, swapWith, size); // Fix the child if needed
}

void sort(std::vector<unsigned int>& data)
{
    countOps(3); // access size, binary shift, create and initialize i
    countOps(2); // access i, compare
    // Let's say data is a heap
    for (int i = data.size() >> 1; i >= 0; --i) // Loop though elements backwards. Elements without children (with index more than size / 2) do not need to be checked
    {
        bubbleDown(data, i, data.size()); // Bubble the element down to fix the heap
        countOps(3); // access i, decrement i, assign i
        countOps(2); // access i, compare
    }
    countOps(2); // access size, create and initialize unsortedSize
    unsigned int unsortedSize = data.size();
    countOps(2); // access unsortedSize, compare
    while (unsortedSize > 1) // While there are elements to sort, continue
    {
        countOps(3); // access unsortedSize, decrement unsortedSize, assign unsortedSize
        --unsortedSize; // The last element will be sorted after this iteration
        countOps(1); // access unsortedSize
        swap(data[0], data[unsortedSize]); // Put last element to the root of the heap
        bubbleDown(data, 0, unsortedSize); // Fix the root of the heap
        countOps(2); // access unsortedSize, compare
    }
}