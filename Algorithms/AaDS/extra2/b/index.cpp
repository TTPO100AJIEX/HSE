#include <iostream>
#include "./HashTable.cpp"

struct hasher
{
    size_t operator()(size_t n) { return 0; }
};

int main()
{
    HashTable<size_t, int, hasher> table(100, 16, hasher{ });
    for (size_t i = 0; i < 200; i++)
    {
        table.insert(i, i);
        std::cout << table.size() << " " << table.capacity() << std::endl;
    }
    std::cout << *table.find(5) << std::endl;
    std::cout << table.size() << " " << table.capacity() << std::endl;
    for (size_t i = 0; i < 1e4; i++)
    {
        std::string test = "@";
        test += (char)(i);
        table.insert(i, i + 1);
    }
    std::cout << *table.find(5) << std::endl;
    std::cout << table.size() << " " << table.capacity() << std::endl;

    for (size_t i = 0; i < 1000; i++)
    {
        //std::cout << table[i].key << " ";
    }
}