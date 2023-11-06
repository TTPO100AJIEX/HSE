#include <cstddef>
#include <algorithm>

class Tree
{
private:
    int value = 0;
    Tree* left = nullptr;
    Tree* right = nullptr;
    size_t height = 0;

public:
    Tree() = default;

    void insert(int value);
    inline size_t getHeight() const;

    ~Tree();
};

void Tree::insert(int value)
{
    if (this->height == 0) { this->value = value; this->height = 1; return; }
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
    size_t left_size = 0, right_size = 0;
    if (this->left != nullptr) left_size = this->left->height;
    if (this->right != nullptr) right_size = this->right->height;
    this->height = std::max(left_size, right_size) + 1;
}

inline size_t Tree::getHeight() const { return this->height; }

Tree::~Tree()
{
    delete this->left;
    delete this->right;
}