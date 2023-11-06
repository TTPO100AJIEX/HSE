set -x
index_file="BaAA/week15/c/index.cpp"
solution_file="BaAA/week15/c/index.cpp"
formatted_file="BaAA/week15/c/formatted.cpp"

rm $formatted_file
cp $solution_file solution_save.cpp

clang-format -i $solution_file
clang-tidy $solution_file -fix-errors -extra-arg=-std=c++17 --
clang-format -i $solution_file
g++ $index_file -fsanitize=address,undefined -fno-sanitize-recover=all -std=c++17 -O2 -Wall -Werror -Wsign-compare -o solution.exe

mv $solution_file $formatted_file
cp solution_save.cpp $solution_file
rm solution_save.cpp

echo "---------------------------------------------------------------------------------------------------------------------------"
cpplint $formatted_file
echo "---------------------------------------------------------------------------------------------------------------------------"

./solution.exe
rm solution.exe