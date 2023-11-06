/*
1) АиСД-2, 2023, задание 5
2) Абрамов Александр Сергеевич БПИ213
3) Visual Studio Code
4) Реализованы 13 алгоритмов сортировки массива, программы генерации тестовых данных и проведения тестирования,
    а также измерения времени выполнения и количества произведённых элементарных операций.
    Реализованы программы обработки полученных данных: формирования корректных xlsx-файлов и построения графиков.
5) Если я правильно понял задание, то все сделано)
*/
const data = {
    "RANDOM_SMALL": new Array(4100).fill(null).map(e => Math.floor(Math.random() * 5)),
    "RANDOM_BIG": new Array(4100).fill(null).map(e => Math.floor(Math.random() * 4000)),
    "ALMOST_SORTED": new Array(4100).fill(null).map((_, i) => i + 1),
    "BACKWARDS_SORTED": new Array(4100).fill(null).map((_, i) => i + 1).reverse()
};
for (let i = 0; i < data.ALMOST_SORTED.length - 50; i += 50)
{
    for (let swapCounter = 0; swapCounter < 50 / 8; swapCounter++) // 50/8 = 6 swaps in every 50 elements
    {
        let ind1 = Math.floor(Math.random() * 50) + i, ind2 = Math.floor(Math.random() * 50) + i;
        [ data.ALMOST_SORTED[ind1], data.ALMOST_SORTED[ind2] ] = [ data.ALMOST_SORTED[ind2], data.ALMOST_SORTED[ind1] ];
    }
}

function generateTest(size, type)
{
    let arr = data[type].slice(0, size);
    return {
        input: `${size}\n` + arr.join(' '),
        output: arr.sort((a, b) => a - b).join(' ')
    };
}

import { groups } from "./config.js";
import fs from 'fs';
for (let groupIndex = 0; groupIndex < groups.length; groupIndex++)
{
    if (fs.existsSync(`tests/group${groupIndex + 1}`)) fs.rmSync(`tests/group${groupIndex + 1}`, { recursive: true });
    const group = groups[groupIndex];
    for (let testIndex = 0; testIndex < group.length; testIndex++)
    {
        const test = group[testIndex], { input, output } = generateTest(test.size, test.type);
        fs.mkdirSync(`tests/group${groupIndex + 1}/test${testIndex + 1}`, { recursive: true });
        fs.writeFileSync(`tests/group${groupIndex + 1}/test${testIndex + 1}/in.in`, input, "utf-8");
        fs.writeFileSync(`tests/group${groupIndex + 1}/test${testIndex + 1}/out.out`, output, "utf-8");
        fs.writeFileSync(`tests/group${groupIndex + 1}/test${testIndex + 1}/descirption.txt`, `${test.size} ${test.type}`, "utf-8");
    }
}