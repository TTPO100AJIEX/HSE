#include <iostream>
#include <utility>
#include <algorithm>

class Tree {
private:
    int value_ = 0;
    Tree* left_ = nullptr;
    Tree* right_ = nullptr;

public:
    Tree() = default;

    void insert(int value);
    const std::pair<unsigned int, bool> check() const;

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

const std::pair<unsigned int, bool> Tree::check() const {
    unsigned int left_height = 0, right_height = 0;
    if (this->left_ != nullptr) {
        const std::pair<unsigned int, bool> left_status = this->left_->check();
        if (!left_status.second) {
            return {0, false};
        }
        left_height = left_status.first;
    }
    if (this->right_ != nullptr) {
        const std::pair<unsigned int, bool> right_status = this->right_->check();
        if (!right_status.second) {
            return {0, false};
        }
        right_height = right_status.first;
    }
    int diff = static_cast<int>(left_height) - static_cast<int>(right_height);
    return {std::max(left_height, right_height) + 1, diff == -1 || diff == 0 || diff == 1};
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
    std::cout << (tree.check().second ? "YES" : "NO");

    return 0;
}