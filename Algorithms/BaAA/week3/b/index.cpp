#include <ios>
#include <iostream>
#include <vector>
#include <algorithm>

void heapSort(std::vector<int>& data)
{
    std::make_heap(data.begin(), data.end());
    std::vector<int>::iterator end = data.end();
    while (end != data.begin() + 1)
    {
        std::pop_heap(data.begin(), end);
        end--;
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n; std::cin >> n;
    std::vector<int> data(n);
    for (std::vector<int>::iterator now = data.begin(); now != data.end(); now++) { std::cin >> *now; }
    heapSort(data);
    for (std::vector<int>::iterator now = data.begin(); now != data.end(); now++) { std::cout << *now << " "; }
}