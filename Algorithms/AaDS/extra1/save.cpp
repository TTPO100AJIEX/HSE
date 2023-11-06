#include <iostream>
#include <cmath>
#include <list>
#include <vector>
#include <utility>

class RootDecomposition
{
public:
    enum Operation { SUM, XOR };

private:
    Operation operation;
    int mod;
    std::list<int> data;
    std::vector< std::pair<int, std::list<int>::iterator> > parts;

public:
    RootDecomposition(Operation operation, int mod, std::istream& instream);
    
    void insert(unsigned int index, int value);
    void erase(unsigned int index);
    void change(unsigned int index, int value);
    int at(unsigned int index);
    int compute(unsigned int left, unsigned int right);
    unsigned int size();
    void print();
};

RootDecomposition::RootDecomposition(Operation operation, int mod, std::istream& instream)
{
    this->operation = operation; this->mod = mod;
    unsigned int size; std::cin >> size;
    this->parts.resize(sqrt(size - 1) + 1, { 0, this->data.end() });
    std::list<int>::iterator last_element = this->data.begin();
    for (unsigned int i = 0; i < size; i++)
    {
        int number; instream >> number;
        this->data.push_back(number);
        if (i == 0) last_element = this->data.begin();
        else last_element++;

        if (this->operation == Operation::SUM) this->parts[i / this->parts.size()].first = (this->parts[i / this->parts.size()].first + number) % this->mod;
        if (this->operation == Operation::XOR) this->parts[i / this->parts.size()].first ^= number;

        if (i % this->parts.size() == 0) this->parts[i / this->parts.size()].second = last_element;
    }
}

void RootDecomposition::insert(unsigned int index, int value)
{
    if (this->data.size() == index)
    {
        this->data.push_back(value);
    }
    else
    {
        unsigned int part = index / this->parts.size(); index %= this->parts.size();
        std::list<int>::iterator it = this->parts[part].second;
        while ((index--) != 0) it++;
        this->data.insert(it, value);
    }

    this->parts.resize(sqrt(this->data.size() - 1) + 1);
    for (unsigned int i = 0; i < this->parts.size(); i++) this->parts[i] = { 0, this->data.end() };
    std::list<int>::iterator last_element = this->data.begin();
    for (unsigned int i = 0; i < this->data.size(); i++, last_element++)
    {
        if (i % this->parts.size() == 0) this->parts[i / this->parts.size()] = { 0, last_element };

        if (this->operation == Operation::SUM) this->parts[i / this->parts.size()].first = (this->parts[i / this->parts.size()].first + (*last_element)) % this->mod;
        if (this->operation == Operation::XOR) this->parts[i / this->parts.size()].first ^= (*last_element);
    }
}
void RootDecomposition::erase(unsigned int index)
{
    unsigned int part = index / this->parts.size(); index %= this->parts.size();
    std::list<int>::iterator it = this->parts[part].second;
    while ((index--) != 0) it++;
    this->data.erase(it);
    if (this->data.size() == 0) return;

    this->parts.resize(sqrt(this->data.size() - 1) + 1);
    for (unsigned int i = 0; i < this->parts.size(); i++) this->parts[i] = { 0, this->data.end() };
    std::list<int>::iterator last_element = this->data.begin();
    for (unsigned int i = 0; i < this->data.size(); i++, last_element++)
    {
        if (i % this->parts.size() == 0) this->parts[i / this->parts.size()] = { 0, last_element };

        if (this->operation == Operation::SUM) this->parts[i / this->parts.size()].first = (this->parts[i / this->parts.size()].first + (*last_element)) % this->mod;
        if (this->operation == Operation::XOR) this->parts[i / this->parts.size()].first ^= (*last_element);
    }
}
void RootDecomposition::change(unsigned int index, int value)
{
    unsigned int part = index / this->parts.size(); index %= this->parts.size();
    std::list<int>::iterator it = this->parts[part].second;
    while ((index--) != 0) it++;

    if (this->operation == Operation::SUM)
    {
        this->parts[part].first -= *it;
        this->parts[part].first = (this->parts[part].first + value) % this->mod;
    }
    if (this->operation == Operation::XOR)
    {
        this->parts[part].first ^= *it;
        this->parts[part].first ^= value;
    }
    *it = value;
}
int RootDecomposition::at(unsigned int index)
{
    unsigned int part = index / this->parts.size(); index %= this->parts.size();
    std::list<int>::iterator it = this->parts[part].second;
    while ((index--) != 0) it++;
    return(*it);
}
int RootDecomposition::compute(unsigned int left, unsigned int right)
{
    unsigned int left_part = left / this->parts.size(); left %= this->parts.size();
    std::list<int>::iterator left_it = this->parts[left_part].second;
    while ((left--) != 0) left_it++;
    unsigned int right_part = right / this->parts.size(); right %= this->parts.size();
    std::list<int>::iterator right_it = this->parts[right_part].second;
    while ((right--) != 0) right_it++;

    int answer = 0;
    while (left_it != right_it)
    {
        if (this->operation == Operation::SUM) answer = (answer + *(left_it++)) % this->mod;
        if (this->operation == Operation::XOR) answer ^= *(left_it++);
    }
    if (this->operation == Operation::SUM) answer = (answer + *(left_it++)) % this->mod;
    if (this->operation == Operation::XOR) answer ^= *(left_it++);
    
    if (this->operation == Operation::SUM && answer < 0) answer += this->mod;
    return answer;

    /*std::list<int>::iterator left_it = this->parts[left_part].second;
    for (unsigned int i = 1; i <= left_index; i++) left_it++;
    std::list<int>::iterator right_it = this->parts[right_part].second;

    if (left_part == right_part)
    {
        for (unsigned int i = left_index; i <= right_index; i++)
        {
            answer = (this->operation == RootDecomposition::Operation::SUM) ? ((answer + *left_it) % this->mod) : (answer ^ *left_it);
            left_it++;
        }
    }
    else
    {
        for (unsigned int i = left_part + 1; i < right_part; i++) { answer += this->parts[i].first; }
        while (left_index != this->part_size)
        {
            answer = (this->operation == RootDecomposition::Operation::SUM) ? ((answer + *left_it) % this->mod) : (answer ^ *left_it);
            left_it++; left_index++;
        }
        right_index++;
        do
        {
            answer = (this->operation == RootDecomposition::Operation::SUM) ? ((answer + *right_it) % this->mod) : (answer ^ *right_it);
            right_it++; right_index--;
        } while (right_index != 0);
    }
    if (this->operation == RootDecomposition::Operation::SUM && answer < 0) answer += this->mod;
    return answer;*/
}
unsigned int RootDecomposition::size() { return this->data.size(); }
void RootDecomposition::print()
{
    std::cout << "------------------------------------\n";
    for (std::list<int>::iterator it = this->data.begin(); it != this->data.end(); it++) std::cout << *it << " ";
    std::cout << "\n";
    for (std::vector< std::pair<int, std::list<int>::iterator> >::iterator it = this->parts.begin(); it != this->parts.end(); it++)
    {
        //std::cout << it->first << "|" << *(it->second) << " ";
    }
    std::cout << "------------------------------------\n";
}

int main(void)
{
    //std::ios::sync_with_stdio(false);
    //std::cin.tie(nullptr);

    char op; std::cin >> op;
    int mod = 1; if (op == '+') std::cin >> mod;
    RootDecomposition solver(op == '+' ? RootDecomposition::Operation::SUM : RootDecomposition::Operation::XOR, mod, std::cin);

    unsigned int queries; std::cin >> queries;
    std::string command; unsigned int i, l, r; int x;
    for (unsigned int query = 0; query < queries; query++)
    {
        std::cin >> command;
        if (command == "insert") { std::cin >> i >> x; solver.insert(i - 1, x); }
        if (command == "erase") { std::cin >> i; solver.erase(i - 1); }
        if (command == "change") { std::cin >> i >> x; solver.change(i - 1, x); }
        if (command == "at") { std::cin >> i; std::cout << solver.at(i - 1) << "\n"; }
        if (command == "compute") { std::cin >> l >> r; std::cout << solver.compute(l - 1, r - 1) << "\n"; }
        if (command == "size") { std::cout << solver.size() << "\n"; }
        
        if (command == "print") { solver.print(); }
    }

    return 0;
}