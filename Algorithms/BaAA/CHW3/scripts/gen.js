import fs from 'fs';
import { vertexes, MIN_EDGE_WEIGHT, MAX_EDGE_WEIGHT } from "./config.js";

fs.rmSync("tests", { recursive: true, force: true });

const graphGenerators = [
    { name: "full", generator: generateFullGraph },
    { name: "average", generator: generateAverageGraph },
    { name: "sparse", generator: generateSparseGraph }
];
for (const vertexesAmount of vertexes)
{
    for (const { name, generator } of graphGenerators)
    {
        fs.mkdirSync(`tests/${name}`, { recursive: true });
        const edges = generator(vertexesAmount);
        const edgesString = edges.map(edge => edge.join(' ')).join('\n');
        const from = Math.floor(Math.random() * vertexesAmount), to = Math.floor(Math.random() * vertexesAmount);
        fs.writeFileSync(`tests/${name}/${vertexesAmount}.txt`, `${vertexesAmount} ${edges.length}\n\n${edgesString}\n\n${from} ${to}`, "utf-8");        
    }
}

function edgeWeight() { return Math.floor(Math.random() * (MAX_EDGE_WEIGHT - MIN_EDGE_WEIGHT + 1)) + MIN_EDGE_WEIGHT; }

function generateFullGraph(vertexesAmount)
{
    let edges = [ ];
    for (let i = 0; i < vertexesAmount; i++) for (let j = i + 1; j < vertexesAmount; j++) edges.push([ i, j, edgeWeight() ]);
    return edges;
}

function generateAverageGraph(vertexesAmount)
{
    const edges = generateSparseGraph(vertexesAmount);
    const remainingEdges = generateFullGraph(vertexesAmount).filter(edge => !edges.find(e => e[0] == edge[0] && e[1] == edge[1]));
    remainingEdges.sort((a, b) => Math.random() - 0.5);
    return edges.concat(remainingEdges.slice(0, 0.45 * (edges.length + remainingEdges.length) - edges.length + 1));
}

function generateSparseGraph(vertexesAmount)
{
    const edges = [ [ 0, 0, edgeWeight() ] ];
    for (let to = 1; to < vertexesAmount; to++) edges.push([ edges[Math.floor(Math.random() * edges.length)][1], to, edgeWeight() ]);
    return edges.slice(1);
}