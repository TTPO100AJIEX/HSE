/*
1) АиСД-2, 2023, задание 5
2) Абрамов Александр Сергеевич БПИ213
3) Visual Studio Code
4) Реализованы 13 алгоритмов сортировки массива, программы генерации тестовых данных и проведения тестирования,
    а также измерения времени выполнения и количества произведённых элементарных операций.
    Реализованы программы обработки полученных данных: формирования корректных xlsx-файлов и построения графиков.
5) Если я правильно понял задание, то все сделано)
*/
import { algorithms, groups, runs } from './config.js';
import { execSync, execFile } from 'child_process';
import fs from 'fs';
const MODE = process.argv.at(-1);
if (fs.existsSync(`bin/${MODE}`)) fs.rmSync(`bin/${MODE}`, { recursive: true });
fs.mkdirSync(`bin/${MODE}`);
function runFile(file, params)
{
    return new Promise((resolve, reject) =>
    {
        execFile(file, params, (error, stdout, stderr) =>
        {
            if (stderr) reject();
            else resolve(stdout);
        });
    });
}

const benchamrkResult = { };
for (const algorithm of algorithms)
{
    switch (MODE)
    {
        case "DEBUG": case "TEST":
            execSync(`g++ sort/index.cpp -D${algorithm} -D${MODE} -fsanitize=address,undefined -fno-sanitize-recover=all -std=c++17 -Werror -Wsign-compare -o bin/${MODE}/${algorithm}.exe`);
            break;
        case "COUNT_ELEMENTARY_OPERATIONS":
            execSync(`g++ sort/index.cpp -D${algorithm} -D${MODE} -std=c++17 -o bin/${MODE}/${algorithm}.exe`);
            break;
        case "COUNT_TIME":
            execSync(`g++ sort/index.cpp -D${algorithm} -D${MODE} -std=c++17 -O2 -o bin/${MODE}/${algorithm}.exe`);
            break;
    }
    console.log(`-------------------${algorithm}-------------------`);
    benchamrkResult[algorithm] = [ ];
    for (let groupIndex = 0; groupIndex < groups.length; groupIndex++)
    {
        switch (MODE)
        {
            case "DEBUG":
            {
                let log = [ ], group_verdict = "OK";
                for (let testIndex = 0; testIndex < groups[groupIndex].length; testIndex++)
                {
                    try {
                        const output = await runFile(`./bin/${MODE}/${algorithm}.exe`, [ `tests/group${groupIndex + 1}/test${testIndex + 1}/in.in` ]);
                        const answer = fs.readFileSync(`tests/group${groupIndex + 1}/test${testIndex + 1}/out.out`, "utf-8");
                        if (output == answer) log.push(`Test ${testIndex + 1}: ✅ OK`);
                        else { group_verdict = "WA"; log.push(`Test ${testIndex + 1}: ❌ WA`); }
                    } catch(err) { group_verdict = "RE"; log.push(`Test ${testIndex + 1}: ⚠️  RE`); }
                }
                if (group_verdict == "OK") console.log(`Группа ${groupIndex + 1}: ✅ OK`);
                if (group_verdict == "RE") console.log(`Группа ${groupIndex + 1}: ⚠️  RE`);
                if (group_verdict == "WA") console.log(`Группа ${groupIndex + 1}: ❌ WA`);
                console.log(log.map(l => "    " + l).join("\n"));
                break;
            }
            case "TEST":
            {
                let group_verdict = "OK";
                for (let testIndex = 0; testIndex < groups[groupIndex].length; testIndex++)
                {
                    try {
                        const output = await runFile(`./bin/${MODE}/${algorithm}.exe`, [ `tests/group${groupIndex + 1}/test${testIndex + 1}/in.in` ]);
                        const answer = fs.readFileSync(`tests/group${groupIndex + 1}/test${testIndex + 1}/out.out`, "utf-8");
                        if (output != answer) { group_verdict = "WA"; break; }
                    }
                    catch(err) { group_verdict = "RE"; break; }
                }
                if (group_verdict == "OK") console.log(`Группа ${groupIndex + 1}: ✅ OK`);
                if (group_verdict == "RE") console.log(`Группа ${groupIndex + 1}: ⚠️  RE`);
                if (group_verdict == "WA") console.log(`Группа ${groupIndex + 1}: ❌ WA`);
                break;
            }
            case "COUNT_TIME":
            {
                const groupResult = [ ];
                for (let testIndex = 0; testIndex < groups[groupIndex].length; testIndex++)
                {
                    groupResult.push([ ]);
                    for (let runIndex = 0; runIndex < runs; runIndex++)
                    {
                        const output = await runFile(`./bin/${MODE}/${algorithm}.exe`, [ `tests/group${groupIndex + 1}/test${testIndex + 1}/in.in` ]);
                        groupResult.at(-1).push(Number(output.slice(output.indexOf(":") + 2, -2)));
                    }
                }
                benchamrkResult[algorithm].push(groupResult);
                break;
            }
            case "COUNT_ELEMENTARY_OPERATIONS":
            {
                const groupResult = [ ];
                for (let testIndex = 0; testIndex < groups[groupIndex].length; testIndex++)
                {
                    groupResult.push([ ]);
                    const output = await runFile(`./bin/${MODE}/${algorithm}.exe`, [ `tests/group${groupIndex + 1}/test${testIndex + 1}/in.in` ]);
                    groupResult.at(-1).push(Number(output.slice(output.indexOf(":") + 2)));
                }
                benchamrkResult[algorithm].push(groupResult);
                break;
            }
        }
    }
}

if (MODE == "COUNT_TIME" || MODE == "COUNT_ELEMENTARY_OPERATIONS") fs.writeFileSync(`report/data/${MODE}.json`, JSON.stringify(benchamrkResult, null, 4), "utf-8");