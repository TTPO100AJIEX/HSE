#include <iostream>
#include <utility>
#include <algorithm>

class Tree
{
private:
    int value = 0;
    Tree* left = nullptr;
    Tree* right = nullptr;

public:
    Tree() = default;

    void insert(int value);
    const std::pair<unsigned int, bool> check() const;

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

const std::pair<unsigned int, bool> Tree::check() const
{
    unsigned int left_height = 0, right_height = 0;
    if (this->left != nullptr)
    {
        const std::pair<unsigned int, bool> left_status = this->left->check();
        if (!left_status.second) return { 0, false };
        left_height = left_status.first;
    }
    if (this->right != nullptr)
    {
        const std::pair<unsigned int, bool> right_status = this->right->check();
        if (!right_status.second) return { 0, false };
        right_height = right_status.first;
    }
    int diff = (int)(left_height) - (int)(right_height);
    return { std::max(left_height, right_height) + 1, diff == -1 || diff == 0 || diff == 1 };
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
    
    Tree tree;
    int number;
    std::cin >> number;
    while (number != 0)
    {
        tree.insert(number);
        std::cin >> number;
    }
    std::cout << (tree.check().second ? "YES" : "NO");
    
    return 0;
}