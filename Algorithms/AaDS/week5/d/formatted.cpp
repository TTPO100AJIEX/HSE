#include <iostream>
#include <cstddef>

class Tree {
private:
    int value_ = 0;
    Tree* left_ = nullptr;
    Tree* right_ = nullptr;

public:
    Tree() = default;

    void insert(int value);
    void printLeaves() const;

    ~Tree();
};

void Tree::insert(int value) {
    if (this->value_ == 0) {
        this->value_ = value;
        return;
    }
    if (value < this->value_) {
        if (this->left_ == nullptr) {
            this->left_ = new Tree();
        }
        this->left_->insert(value);
    }
    if (value > this->value_) {
        if (this->right_ == nullptr) {
            this->right_ = new Tree();
        }
        this->right_->insert(value);
    }
}
void Tree::printLeaves() const {
    if (this->left_ == nullptr && this->right_ == nullptr) {
        if (this->value_ != 0) {
            std::cout << this->value_ << "\n";
        }
        return;
    }
    if (this->left_ != nullptr) {
        this->left_->printLeaves();
    }
    if (this->right_ != nullptr) {
        this->right_->printLeaves();
    }
}
Tree::~Tree() {
    delete this->left_;
    delete this->right_;
}

int main(void) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    Tree tree;
    int number;
    std::cin >> number;
    while (number != 0) {
        tree.insert(number);
        std::cin >> number;
    }
    tree.printLeaves();
    return 0;
}