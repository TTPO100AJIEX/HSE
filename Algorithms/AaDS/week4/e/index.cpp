#include <iostream>
#include <stack>
#include <algorithm>
#include <cstdint>

template <typename elements_type>
class MinimumQueue
{
public:
    elements_type min() const;
    void push(elements_type value);
    void pop();

private:
    void pushToStack(elements_type value, bool stack_index);

    std::stack < std::pair <elements_type, elements_type> > push_stack; 
    std::stack < std::pair <elements_type, elements_type> > pop_stack;
};

template<typename elements_type>
void MinimumQueue<elements_type>::pushToStack(elements_type value, bool stack_index)
{
    std::stack< std::pair <elements_type, elements_type> >& stack = (stack_index == 1) ? this->push_stack : this->pop_stack;
    elements_type current_minimum = value;
    if (!stack.empty()) current_minimum = std::min(current_minimum, stack.top().second);
    stack.emplace(value, current_minimum);
}


template<typename elements_type>
elements_type MinimumQueue<elements_type>::min() const
{
    if (this->push_stack.empty()) return (this->pop_stack.top().second);
    if (this->pop_stack.empty()) return (this->push_stack.top().second);
    return (std::min(this->push_stack.top().second, this->pop_stack.top().second));
}

template<typename elements_type>
void MinimumQueue<elements_type>::push(elements_type value)
{
    this->pushToStack(value, 1);
}

template<typename elements_type>
void MinimumQueue<elements_type>::pop()
{
    if (this->pop_stack.empty())
    {
        while (!this->push_stack.empty())
        {
            this->pushToStack(this->push_stack.top().first, 2);
            this->push_stack.pop();
        }
    }
    this->pop_stack.pop();
}

int main(void)
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n, k;
    std::cin >> n >> k;
    MinimumQueue<int16_t> solver;

    for (unsigned int i = 0; i < k; i++)
    {
        int16_t number;
        std::cin >> number;
        solver.push(number);
    }
    for (unsigned int i = k; i < n; i++)
    {
        std::cout << solver.min() << " ";
        solver.pop();

        int16_t number;
        std::cin >> number;
        solver.push(number);
    }
    std::cout << solver.min() << " ";

    return 0;
}