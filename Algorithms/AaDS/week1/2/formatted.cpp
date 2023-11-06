#include <list>

class Deque {
public:
    Deque();

    void pushBack(int value);
    void pushMiddle(int value);

    int popFront();

    int front() const;
    int back() const;
    size_t size() const;

private:
    std::list<int> storage_;
    std::list<int>::iterator middle_iterator_;
};

Deque::Deque() {
    this->middle_iterator_ = this->storage_.begin();
}

void Deque::pushBack(int value) {
    this->storage_.push_back(value);
    if ((this->size() & 1) == 1) {
        this->middle_iterator_++;
        return;
    }
    if (this->size() == 2) {
        this->middle_iterator_ = this->storage_.end();
        this->middle_iterator_--;
        return;
    }
}
void Deque::pushMiddle(int value) {
    this->storage_.insert(this->middle_iterator_, value);
    if ((this->size() & 1) == 0) {
        this->middle_iterator_--;
}
}

int Deque::popFront() {
    int item = this->storage_.front();
    this->storage_.pop_front();
    if ((this->size() & 1) == 1) {
        this->middle_iterator_++;
}
    return (item);
}

int Deque::front() const {
    return (this->storage_.front());
}
int Deque::back() const {
    return (this->storage_.back());
}
size_t Deque::size() const {
    return (this->storage_.size());
}