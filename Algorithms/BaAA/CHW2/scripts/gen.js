import fs from 'fs';
import { texts, alphabets, patterns, substitutionSymbols } from "./config.js";


fs.rmSync("tests", { recursive: true, force: true });
for (const textSize of texts)
{
    for (const alphabet of alphabets)
    {
        const text = generateText(textSize, alphabet);
        const folder = `tests/${textSize}-${alphabet}`;
        fs.mkdirSync(folder, { recursive: true });
        fs.writeFileSync(`${folder}/text.txt`, text, "utf-8");
        const patternStartIndex = Math.floor(Math.random() * (text.length - Math.max(...patterns)));
        for (const patternSize of patterns)
        {
            for (const substitutionSymbolsAmount of substitutionSymbols)
            {
                const pattern = text.substring(patternStartIndex, patternStartIndex + patternSize);
                const { test, answer } = generateTest(text, pattern, substitutionSymbolsAmount);
                const testFolder = `${folder}/patterns-${substitutionSymbolsAmount}/${patternSize}`;
                fs.mkdirSync(testFolder, { recursive: true });
                fs.writeFileSync(`${testFolder}/in.txt`, test, "utf-8");
                fs.writeFileSync(`${testFolder}/out.txt`, answer, "utf-8");
            }
        }
    }
}



function generateText(textSize, alphabet)
{
    return new Array(textSize).fill(undefined).map((_, index) => alphabet[Math.floor(Math.random() * alphabet.length)]).join('');
}
function generateTest(text, pattern, substitutionSymbolsAmount)
{
    pattern = pattern.split('');
    for (let i = 0; i < substitutionSymbolsAmount; i++)
    {
        const index = Math.floor(Math.random() * pattern.length);
        if (pattern[index] == '?') i--;
        pattern[index] = '?';
    }
    const test = pattern.join('');

    const regex = new RegExp(test.replaceAll("?", "."), "g");
    let answer = [ ], match;
    while (match = regex.exec(text))
    {
        answer.push(match.index);
        regex.lastIndex = match.index + 1;
    }

    return { test, answer: answer.join(' ') };
}