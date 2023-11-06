#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <set>
#include <chrono>

extern std::set<unsigned int> search(std::string_view text, std::string_view pattern);

int main(int argc, char** argv)
{
    if (argc < 3) throw "Not enought command line arguments specified";

    std::string text, pattern;
    std::ifstream fin;

    fin.open(argv[1]);
    fin >> text;
    fin.close();
    
    fin.open(argv[2]);
    fin >> pattern;
    fin.close();


    // Choose the best clock
    using clock = std::conditional_t<std::chrono::high_resolution_clock::is_steady, std::chrono::high_resolution_clock, std::chrono::steady_clock>;
    
    clock::time_point start = clock::now(); // Get the starting timestamp
    const std::set<unsigned int> answer = search(text, pattern);
    clock::time_point end = clock::now(); // Get the ending timestamp
    
    for (std::set<unsigned int>::const_iterator i = answer.begin(); i != answer.end(); ++i)
    {
        if (*i == *(answer.rbegin())) std::cout << *i << '\n';
        else std::cout << *i << ' ';
    }
    std::cout << "Time spent: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << "ns";

    return 0;
}