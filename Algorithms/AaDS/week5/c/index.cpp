#include <iostream>
#include <cstddef>

class Tree
{
private:
    int value = 0;
    Tree* left = nullptr;
    Tree* right = nullptr;

public:
    Tree() = default;

    void insert(int value);
    void print_sorted() const;

    ~Tree();
};

void Tree::insert(int value)
{
    if (this->value == 0) { this->value = value; return; }
    if (value < this->value)
    {
        if (this->left == nullptr) this->left = new Tree();
        this->left->insert(value);
    }
    if (value > this->value)
    {
        if (this->right == nullptr) this->right = new Tree();
        this->right->insert(value);
    }
}
void Tree::print_sorted() const
{
    if (this->left != nullptr) this->left->print_sorted();
    std::cout << this->value << "\n";
    if (this->right != nullptr) this->right->print_sorted();
}
Tree::~Tree()
{
    delete this->left;
    delete this->right;
}

int main(void)
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    Tree tree; int number;
    std::cin >> number;
    while (number != 0)
    {
        tree.insert(number);
        std::cin >> number;
    }
    tree.print_sorted();
    return 0;
}