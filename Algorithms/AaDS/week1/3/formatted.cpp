#include <stack>

class Queue {
public:
    void push(int value);

    int pop();

    int min() const;

private:
    struct Item {
        int value;
        int minBefore;

        Item(int value, int min_before) : value(value), minBefore(min_before){};
    };

    void pushToStack(int value, int stack_index);

    std::stack<Item> stack1_;
    std::stack<Item> stack2_;
};

void Queue::pushToStack(int value, int stack_index) {
    int current_minimum = value;
    std::stack<Item>* stack = (stack_index == 1) ? &(this->stack1_) : &(this->stack2_);
    if (!stack->empty()) {
        current_minimum = std::min(current_minimum, stack->top().minBefore);
    }
    stack->emplace(value, current_minimum);
}
void Queue::push(int value) {
    this->pushToStack(value, 1);
}

int Queue::pop() {
    if (this->stack2_.empty()) {
        while (!this->stack1_.empty()) {
            int value = this->stack1_.top().value;
            this->pushToStack(value, 2);
            this->stack1_.pop();
        }
    }
    int return_value = this->stack2_.top().value;
    this->stack2_.pop();
    return (return_value);
}

int Queue::min() const {
    if (this->stack1_.empty()) {
        return (this->stack2_.top().minBefore);
    }
    if (this->stack2_.empty()) {
        return (this->stack1_.top().minBefore);
    }
    return (std::min(this->stack1_.top().minBefore, this->stack2_.top().minBefore));
}