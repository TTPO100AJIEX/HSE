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
    void print_leaves() const;

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
void Tree::print_leaves() const
{
    if (this->left == nullptr && this->right == nullptr)
    {
        if (this->value != 0) std::cout << this->value << "\n";
        return;
    }
    if (this->left != nullptr) this->left->print_leaves();
    if (this->right != nullptr) this->right->print_leaves();
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
    tree.print_leaves();
    return 0;
}