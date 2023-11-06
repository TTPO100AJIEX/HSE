/*
1) АиСД-2, 2023, задание 5
2) Абрамов Александр Сергеевич БПИ213
3) Visual Studio Code
4) Реализованы 13 алгоритмов сортировки массива, программы генерации тестовых данных и проведения тестирования,
    а также измерения времени выполнения и количества произведённых элементарных операций.
    Реализованы программы обработки полученных данных: формирования корректных xlsx-файлов и построения графиков.
5) Если я правильно понял задание, то все сделано)
*/
import excelJS from 'exceljs';
import data from "../report/data/data.json" assert { type: "json" };
import { groups, types, runs } from './config.js';

let workbook = new excelJS.Workbook();
for (const key in data)
{
    let worksheet = null;
    switch(key)
    {
        case "time": { worksheet = workbook.addWorksheet("Время"); break; }
        case "operations": { worksheet = workbook.addWorksheet("Операции"); break; }
    }

    const columns = groups.reduce((prev, cur) => prev + cur.length, 0) / types.length;
    worksheet.getRow(1).height = 25; worksheet.getRow(2).height = 20;
    for (let groupIndex = 0, column = 3; groupIndex < groups.length; groupIndex++)
    {
        const group = groups[groupIndex]; worksheet.mergeCells(1, column, 1, column + group.length / types.length - 1);
        let heading = worksheet.getRow(1).getCell(column); heading.value = `Группа ${groupIndex + 1}`; heading.alignment = { vertical: "middle" }; heading.font = { size: 18, bold: true };
        
        for (const size of [ ...new Set(group.map(test => test.size)) ])
        {
            let subheading = worksheet.getRow(2).getCell(column); subheading.value = size; subheading.alignment = { vertical: "middle", horizontal: "center" }; subheading.font = { size: 12, bold: true };
            column++;
        }
    }
    
    let nextRow = 3; worksheet.getColumn("A").width = 12.5; worksheet.getColumn("B").width = 32.5;
    for (const algorithm in data[key])
    {
        worksheet.mergeCells(nextRow, 1, nextRow + 3, 1);
        let head = worksheet.getRow(nextRow).getCell(1); head.alignment = { vertical: "middle", horizontal: "center", textRotation: 90, wrapText: true }; head.font = { size: 12, bold: true };
        switch (algorithm)
        {
            case "BINARY_INSERTION": { head.value = "Бинарными вставками"; break; }
            case "BUBBLE_1": { head.value = "Пузырьком с условием Айверсона 1"; break; }
            case "BUBBLE_2": { head.value = "Пузырьком с условием Айверсона 1+2"; break; }
            case "BUBBLE": { head.value = "Сортировка пузырьком"; break; }
            case "COUNTING": { head.value = "Подсчетом (устойчивая)"; break; }
            case "HEAP": { head.value = "Пирамидальная сортировка"; break; }
            case "INSERTION": { head.value = "Простыми вставками"; break; }
            case "MERGE": { head.value = "Сортировка слиянием"; break; }
            case "QUICK": { head.value = "Быстрая (первый опорный)"; break; }
            case "RADIX": { head.value = "Цифровая сортировка"; break; }
            case "SELECTION": { head.value = "Сортировка выбором"; break; }
            case "SHELL_CIUR": { head.value = "Шелла (последовательность Циура)"; break; }
            case "SHELL": { head.value = "Шелла (последовательность Шелла)"; break; }
        }

        for (let typeIndex = 0; typeIndex < types.length; typeIndex++)
        {
            worksheet.getRow(nextRow + typeIndex).height = 17.5;
            let typeHeading = worksheet.getRow(nextRow + typeIndex).getCell(2); typeHeading.alignment = { vertical: "middle" }; typeHeading.font = { size: 12, bold: true };
            switch (types[typeIndex])
            {
                case "RANDOM_SMALL": { typeHeading.value = "Случайные 0-5"; break; }
                case "RANDOM_BIG": { typeHeading.value = "Случайные 0-4000"; break; }
                case "ALMOST_SORTED": { typeHeading.value = "Почти отсортированный"; break; }
                case "BACKWARDS_SORTED": { typeHeading.value = "Обратно отсортированный"; break; }
            }

            for (let groupIndex = 0, nextColumn = 3; groupIndex < data[key][algorithm].length; groupIndex++)
            {
                const testData = Object.entries(data[key][algorithm][groupIndex][types[typeIndex]]).sort((a, b) => a[0] - b[0]);
                for (const test of testData){ let testCell = worksheet.getRow(nextRow + typeIndex).getCell(nextColumn++); testCell.value = test[1]; testCell.alignment = { vertical: "middle", horizontal: "center" }; testCell.font = { size: 10 }; }
            }
        }
        

        worksheet.mergeCells(nextRow + 4, 1, nextRow + 4, columns + 2);
        worksheet.getRow(nextRow + 4).getCell(1).fill = { type: "pattern", pattern: "solid", bgColor: { argb: 'FF000000' } };
        worksheet.getRow(nextRow + 4).height = 10;

        nextRow += 5;
    }
}
await workbook.xlsx.writeFile("report/data/results.xlsx");