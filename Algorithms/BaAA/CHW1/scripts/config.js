/*
1) АиСД-2, 2023, задание 5
2) Абрамов Александр Сергеевич БПИ213
3) Visual Studio Code
4) Реализованы 13 алгоритмов сортировки массива, программы генерации тестовых данных и проведения тестирования,
    а также измерения времени выполнения и количества произведённых элементарных операций.
    Реализованы программы обработки полученных данных: формирования корректных xlsx-файлов и построения графиков.
5) Если я правильно понял задание, то все сделано)
*/
const types = [ "RANDOM_SMALL", "RANDOM_BIG", "ALMOST_SORTED", "BACKWARDS_SORTED" ];
const groups =
[
    new Array(((300 - 50) / 50 + 1) * types.length).fill(null).map((_, i) => ({ size: (Math.floor(i / types.length) + 1) * 50, type: types[i % types.length] })),
    new Array(((4100 - 100) / 100 + 1) * types.length).fill(null).map((_, i) => ({ size: (Math.floor(i / types.length) + 1) * 100, type: types[i % types.length] }))
];
const algorithms =
[
    "BUBBLE",
    "BUBBLE_1",
    "BUBBLE_2",
    "SELECTION",
    "INSERTION",
    "BINARY_INSERTION",
    "MERGE",
    "HEAP", 
    "QUICK",
    "SHELL",
    "SHELL_CIUR",
    "COUNTING",
    "RADIX"
];
const runs = 50;
export { types, groups, algorithms, runs };