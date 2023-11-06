#include <ios>
#include <iostream>
#include <vector>
#include <utility>

class Heap
{
private:
    std::vector <int> data;

    void bubbleUp(unsigned int index)
    {
        if (index == 0) return;
        const unsigned int parent = (index - 1) >> 1;
        if (this->data[parent] > this->data[index])
        {
            std::swap(this->data[parent], this->data[index]);
            this->bubbleUp(parent);
        }
    }
    void bubbleDown(unsigned int index)
    {
        const unsigned int size = this->data.size(), leftIndex = (index << 1) + 1, rightIndex = (index << 1) + 2;
        unsigned int swapWith = size;
        if (leftIndex < size && this->data[index] > this->data[leftIndex]) swapWith = leftIndex;
        if (rightIndex < size && this->data[index] > this->data[rightIndex] && this->data[leftIndex] > this->data[rightIndex]) swapWith = rightIndex;
        if (swapWith == size) return;
        std::swap(this->data[swapWith], this->data[index]);
        this->bubbleDown(swapWith);
    }

public:
    bool empty() const { return this->data.empty(); }

    void insert(const int value)
    {
        this->data.push_back(value);
        this->bubbleUp(this->data.size() - 1);
    }
    int extract()
    {
        int answer = this->data[0];
        this->data[0] = this->data[this->data.size() - 1];
        this->data.pop_back();
        this->bubbleDown(0);
        return answer;
    }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    Heap sorter;
    unsigned int n; std::cin >> n;
    for (unsigned int i = 0; i < n; i++) { int value; std::cin >> value; sorter.insert(value); }
    while (!sorter.empty()) { std::cout << sorter.extract() << " "; }
}