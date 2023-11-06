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
    countOps(3); // access i, access size, loop comparison
    for (unsigned int i = 1; i < data.size(); ++i) // Loop through input
    {
        countOps(3); // access i, create and initialize left and right
        int left = -1, right = i; // The correct position of the current element in the prefix is between left and right

        countOps(4); // access left, access right, subtract, loop comparison
        while (right - left > 1) // Binary search for the position
        {
            countOps(5); // access left, access right, sum, binary shift, create and initialize mid
            int mid = (left + right) >> 1; // Find middle index
            
            countOps(6); // access mid, access i, access data[mid], access data[i], comparison, assignment
            if (data[mid] > data[i]) right = mid; // If middle is too big, move right border
            else left = mid; // Otherwise, move left border

            countOps(4); // access left, access right, subtract, loop comparison
        }

        // Insert the current element in the correct position pointed by right (and move all elements in the subarray to the right)
        countOps(2); // access i, create and initialize j
        countOps(3); // access j, access right, loop comparison
        for (unsigned int j = i; j > (unsigned int)(right); --j) // Loop from the current position to the insertion position
        {
            countOps(2); // access j, subtract 1
            swap(data[j], data[j - 1]); // Swap current and previous elements thus moving all elements one position to the right and putting the current element in its correct position
            countOps(3); // access j, subtract from j, assign j
            countOps(3); // access j, access right, loop comparison
        }
        
        countOps(3); // access i, increment i, assign i
        countOps(3); // access i, access size, loop comparison
    }
}