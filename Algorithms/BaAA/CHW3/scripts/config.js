export const vertexes = [ ];
for (let i = 10; i <= 1010; i += 50) vertexes.push(i);

export const algorithms = [
    "dijkstra-set",
    "dijkstra-set-optimized",
    "dijkstra-queue",
    "dijkstra-queue-delete",
    "ford-bellman",
    "floyd-warshall"
];

export const iterations = 50;

export const MIN_EDGE_WEIGHT = 1, MAX_EDGE_WEIGHT = 10;