import fs from 'fs';
import { execSync, execFileSync } from 'child_process';
import { algorithms, alphabets, patterns, runs, substitutionSymbols, texts } from "./config.js";
const MODE = process.argv.at(-1);


/*----------------------------COMPILE----------------------------*/
const modeFolder = `bin/${MODE}`;
fs.rmSync(modeFolder, { recursive: true, force: true });
fs.mkdirSync(modeFolder, { recursive: true });

for (const algorithm of algorithms)
{
    const executableName = `${modeFolder}/${algorithm}.exe`;
    switch (MODE)
    {
        case "DEBUG": case "TEST":
            execSync(`g++ algorithms/implementations/${algorithm}.cpp algorithms/index.cpp -fsanitize=address,undefined -fno-sanitize-recover=all -std=c++20 -Werror -Wall -Wextra -O2 -o ${executableName}`);
            break;
        case "TIME":
            execSync(`g++ algorithms/implementations/${algorithm}.cpp algorithms/index.cpp -std=c++20 -O2 -o ${executableName}`);
            break;
        default:
            console.error(`Wrong MODE ${MODE} specified!`);
            process.exit(-1);
    }
}


/*----------------------------RUN----------------------------*/
if (MODE == "TEST" || MODE == "DEBUG")
{
    for (const algorithm of algorithms)
    {
        console.log(`-------------------${algorithm.toUpperCase()}-------------------`);

        const executableName = `${modeFolder}/${algorithm}.exe`;
        for (const textSize of texts)
        {
            for (const alphabet of alphabets)
            {
                let testsLog = [ ], testsVerdict = "OK";
                patterns: for (const substitutionSymbolsAmount of substitutionSymbols)
                {
                    for (const patternSize of patterns)
                    {
                        const textFolder = `tests/${textSize}-${alphabet}`, patternFolder = `${textFolder}/patterns-${substitutionSymbolsAmount}/${patternSize}`;
                        const patternInfo = `Pattern of size ${patternSize.toString().padEnd(4, " ")} with ${substitutionSymbolsAmount} substitution symbols`;
                        const answer = fs.readFileSync(`${patternFolder}/out.txt`, "utf-8");
                        try
                        {
                            const [ output, _ ] = execFileSync(executableName, [ `${textFolder}/text.txt`, `${patternFolder}/in.txt` ]).toString().split('\n');
                            if (output == answer)
                            {
                                testsLog.push(`${patternInfo}: ✅ OK`);
                            }
                            else
                            {
                                testsVerdict = "WA";
                                testsLog.push(`${patternInfo}: ❌ WA (${output})`);
                                break patterns;
                            }
                        }
                        catch(err)
                        {
                            testsVerdict = "RE";
                            testsLog.push(`${patternInfo}: ⚠️  RE (${err})`);
                            break patterns;
                        }
                    }
                }

                const textInfo = `Text ${textSize.toString().padEnd(6, " ")} on alphabet ${alphabet.toString().padEnd(4, " ")}`;
                switch(testsVerdict)
                {
                    case "OK": { console.log(`${textInfo}: ✅ OK`); break; }
                    case "WA": { console.log(`${textInfo}: ❌ WA`); break; }
                    case "RE": { console.log(`${textInfo}: ⚠️  RE`); break; }
                }
                if (MODE == "DEBUG") console.log(testsLog.map(logRecord => "    " + logRecord).join("\n"));
            }
        }
    }
}

if (MODE == "TIME")
{
    const timeResult = { };
    for (const textSize of texts)
    {
        for (const alphabet of alphabets)
        {
            const textId = `${textSize}-${alphabet}`;
            console.log(`-------------------${textId}-------------------`);
            timeResult[textId] = { };
            for (const substitutionSymbolsAmount of substitutionSymbols)
            {
                timeResult[textId][substitutionSymbolsAmount] = { };
                for (const patternSize of patterns)
                {
                    timeResult[textId][substitutionSymbolsAmount][patternSize] = { };
                    const textFolder = `tests/${textId}`, patternFolder = `${textFolder}/patterns-${substitutionSymbolsAmount}/${patternSize}`;
                    for (let runIndex = 0; runIndex < runs; runIndex++)
                    {
                        for (const algorithm of algorithms)
                        {
                            const executableName = `${modeFolder}/${algorithm}.exe`;
                            const [ _, time ] = execFileSync(executableName, [ `${textFolder}/text.txt`, `${patternFolder}/in.txt` ]).toString().split('\n');
                            (timeResult[textId][substitutionSymbolsAmount][patternSize][algorithm] ??= [ ]).push(Number(time.slice(time.indexOf(":") + 2, -2)));
                        }
                    }
                }
            }
        }
    }

    fs.writeFileSync("report/data.json", JSON.stringify(timeResult, null, 4), "utf-8");
}