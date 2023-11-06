set index_file=week6\b\index.cpp

g++ %index_file% -std=c++17 -O2 -o solution.exe
start /b /w solution.exe
del solution.exe
PAUSE