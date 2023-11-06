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

#include "./countOps.h"

template <typename T>
void swap(T& a, T& b)
{
    countOps(2); // access a, create and initialize c
    T c = std::move(a); // Save "a" to a temporary variable "c"
    countOps(2); // access b, assign to a
    a = std::move(b); // Assign "b" to "a"
    countOps(2); // access c, assign to b
    b = std::move(c); // Assign the saved value from "c" to "b"
}