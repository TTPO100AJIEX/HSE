#include <cstddef>
#include <vector>
#include <initializer_list>
#include <utility>

template <class ValueType>
class Heap
{
private:
    std::vector <ValueType> data;

    void bubbleUp(unsigned int index)
    {
        if (index == 0) return;
        const unsigned int parent = (index - 1) >> 1;
        if (this->data[parent] < this->data[index])
        {
            std::swap(this->data[parent], this->data[index]);
            this->bubbleUp(parent);
        }
    }
    void bubbleDown(unsigned int index)
    {
        const unsigned int size = this->size(), leftIndex = (index << 1) + 1, rightIndex = (index << 1) + 2;
        unsigned int swapWith = size;
        if (leftIndex < size && this->data[index] < this->data[leftIndex]) swapWith = leftIndex;
        if (rightIndex < size && this->data[index] < this->data[rightIndex] && this->data[leftIndex] < this->data[rightIndex]) swapWith = rightIndex;
        if (swapWith == size) return;
        std::swap(this->data[swapWith], this->data[index]);
        this->bubbleDown(swapWith);
    }

public:
    template <class Iterator> Heap(Iterator begin, Iterator end) { while (begin != end) this->insert(*(begin++)); }
    Heap(std::initializer_list<ValueType> data) { for (const ValueType& value : data) this->insert(value); }

    Heap() = default;
    Heap(const Heap& other) = default;
    Heap(Heap&& other) = default;
    Heap& operator=(const Heap& other) = default;
    Heap& operator=(Heap&& other) = default;
    ~Heap() = default;

    size_t size() const { return this->data.size(); }
    bool empty() const { return this->data.empty(); }

    void insert(const ValueType& value)
    {
        this->data.push_back(value);
        this->bubbleUp(this->size() - 1);
    }
    ValueType extract()
    {
        ValueType answer = this->data[0];
        this->data[0] = this->data[this->size() - 1];
        this->data.pop_back();
        this->bubbleDown(0);
        return answer;
    }
};