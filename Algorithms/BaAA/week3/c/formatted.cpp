#include <cstddef>
#include <vector>
#include <initializer_list>
#include <utility>

template <class ValueType>
class Heap {
private:
    std::vector<ValueType> data_;

    void bubbleUp(unsigned int index) {
        if (index == 0) {
            return;
        }
        const unsigned int parent = (index - 1) >> 1;
        if (this->data_[parent] < this->data_[index]) {
            std::swap(this->data_[parent], this->data_[index]);
            this->bubbleUp(parent);
        }
    }
    void bubbleDown(unsigned int index) {
        const unsigned int size = this->size(), left_index = (index << 1) + 1,
                           right_index = (index << 1) + 2;
        unsigned int swap_with = size;
        if (left_index < size && this->data_[index] < this->data_[left_index]) {
            swap_with = left_index;
        }
        if (right_index < size && this->data_[index] < this->data_[right_index] &&
            this->data_[left_index] < this->data_[right_index]) {
            swap_with = right_index;
        }
        if (swap_with == size) {
            return;
        }
        std::swap(this->data_[swap_with], this->data_[index]);
        this->bubbleDown(swap_with);
    }

public:
    template <class Iterator>
    Heap(Iterator begin, Iterator end) {
        while (begin != end) {
            this->insert(*(begin++));
        }
    }
    Heap(std::initializer_list<ValueType> data) {
        for (const ValueType& value : data) {
            this->insert(value);
        }
    }

    Heap() = default;
    Heap(const Heap& other) = default;
    Heap(Heap&& other) = default;
    Heap& operator=(const Heap& other) = default;
    Heap& operator=(Heap&& other) = default;
    ~Heap() = default;

    size_t size() const {
        return this->data_.size();
    }
    bool empty() const {
        return this->data_.empty();
    }

    void insert(const ValueType& value) {
        this->data_.push_back(value);
        this->bubbleUp(this->size() - 1);
    }
    ValueType extract() {
        ValueType answer = this->data_[0];
        this->data_[0] = this->data_[this->size() - 1];
        this->data_.pop_back();
        this->bubbleDown(0);
        return answer;
    }
};