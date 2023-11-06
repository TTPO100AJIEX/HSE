#include <iostream>
#include <vector>

class Stack {
public:
    void push(int value);
    int pop();

    int back() const;
    size_t size() const;

    void clear();

private:
    std::vector<int> storage_;
};

void Stack::push(int value) {
    storage_.push_back(value);
}
int Stack::pop() {
    if (this->storage_.empty()) {
        throw std::runtime_error("error");
    }
    int item = this->storage_.back();
    this->storage_.pop_back();
    return (item);
}

int Stack::back() const {
    if (this->storage_.empty()) {
        throw std::runtime_error("error");
    }
    return (this->storage_.back());
}
size_t Stack::size() const {
    return (this->storage_.size());
}

void Stack::clear() {
    this->storage_.clear();
}