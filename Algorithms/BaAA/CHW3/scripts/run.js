import fs from 'fs';
import { execFileSync, execSync } from 'child_process';
import { vertexes, algorithms, iterations } from './config.js';

fs.rmSync("bin", { recursive: true, force: true });
fs.mkdirSync("bin", { recursive: true });
for (const algorithm of algorithms) execSync(`g++ algorithms/${algorithm}.cpp -O2 -std=c++20 -Werror -Wall -Wextra -o bin/${algorithm}.exe`);

var result = { vertexes: { }, edges: { } };
for (const vertexesAmount of vertexes)
{
    console.log(vertexesAmount);
    for (const graphType of fs.readdirSync("tests"))
    {
        const edgesAmount = Number(fs.readFileSync(`tests/${graphType}/${vertexesAmount}.txt`, "utf-8").split(/ |\n/)[1]);
        result.vertexes[vertexesAmount] ??= { };
        result.edges[edgesAmount] ??= { };

        result.vertexes[vertexesAmount][graphType] ??= { };
        result.edges[edgesAmount][graphType] ??= { };
        
        const answers = { };
        for (let i = 0; i < iterations; i++)
        {
            for (const algorithm of algorithms)
            {
                result.vertexes[vertexesAmount][graphType][algorithm] ??= [ ];
                result.edges[edgesAmount][graphType][algorithm] ??= [ ];
                answers[algorithm] ??= [ ];
    
                const [ answer, time ] = execFileSync(`bin/${algorithm}.exe`, [ `tests/${graphType}/${vertexesAmount}.txt` ]).toString().split('\n');
                answers[algorithm].push(answer);
                result.vertexes[vertexesAmount][graphType][algorithm].push(Number(time.slice(time.indexOf(":") + 2, -2)));
                result.edges[edgesAmount][graphType][algorithm].push(Number(time.slice(time.indexOf(":") + 2, -2)));
            }
        }
        
        if (new Set(Object.values(answers).flat()).size != 1)
        {
            console.warn("Incorrect answer found!");
            console.info(answers);
        }
    }
}

fs.writeFileSync("report/data.json", JSON.stringify(result, null, 4), "utf-8");