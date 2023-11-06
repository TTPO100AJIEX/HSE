const texts = [ 10000, 100000 ];
const alphabets = [ "01", "abcd" ];
const patterns = [ ];
for (let i = 100; i <= 3000; i += 100) patterns.push(i);
const substitutionSymbols = [ 0, 1, 2, 3, 4, 30 ];

const algorithms = [ "naive", "kmp", "kmp-optimized", "aho-corasick" ];
const runs = 100;

export { texts, alphabets, patterns, substitutionSymbols, algorithms, runs };