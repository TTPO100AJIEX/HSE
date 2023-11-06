set index_file=week6\b\index.cpp
set solution_file=week6\b\AVLTree.cpp

ECHO F | xcopy %solution_file% solution_save.cpp

clang-format -i %solution_file%
clang-tidy %solution_file% -fix-errors
clang-format -i %solution_file%
g++ %index_file% -fno-sanitize-recover=all -std=c++17 -O2 -Wall -Werror -Wsign-compare -o solution.exe

ren %solution_file% formatted.cpp
ECHO F | xcopy /Y solution_save.cpp %solution_file%
del solution_save.cpp

start /b /w solution.exe
del solution.exe

PAUSE