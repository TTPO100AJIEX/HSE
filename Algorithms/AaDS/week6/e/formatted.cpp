#include <iostream>
#include <utility>

class Set {
    struct Node {
        unsigned int size = 1;
        int value;
        Node* left = nullptr;
        Node* right = nullptr;
        Node* parent = nullptr;

        enum class Color { RED, BLACK };
        Color color = Color::RED;
        bool is_double_black = false;

        explicit Node(int value) : value(value){};
        Node(int value, Node* parent) : value(value), parent(parent){};

        bool isLeftChild() const {
            return (this->parent ? (this->parent->left == this) : false);
        }
        bool isRightChild() const {
            return (this->parent ? (this->parent->right == this) : false);
        }
        void remove() {
            if (this->isLeftChild()) {
                this->parent->left = nullptr;
            } else {
                this->parent->right = nullptr;
            }
            delete this;
        }

        void recalculateSize() {
            unsigned int left_size = (this->left ? this->left->size : 0),
                         right_size = (this->right ? this->right->size : 0);
            this->size = 1 + left_size + right_size;
        }
    };
    Node* root_ = nullptr;

    enum class Rotations { LEFT, RIGHT };
    void rotation(Node* root, Rotations type);
    void fixBalance(Node* current);
    void fixDoubleBlack(Node* current);
    void recalculateSize(Node* current);

public:
    int get(unsigned int index);
    void insert(int value);
    void erase(int value);

    void destroy(Node* node) {
        if (!node) {
            return;
        }
        this->destroy(node->left);
        this->destroy(node->right);
        delete node;
    }
    ~Set() {
        this->destroy(this->root_);
    }
};

int main(void) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n;
    std::cin >> n;
    Set solver;
    for (unsigned int i = 0; i < n; ++i) {
        char type;
        int value;
        std::cin >> type >> value;
        switch (type) {
            case '?': {
                std::cout << solver.get(value) << '\n';
                break;
            }
            case '+': {
                solver.insert(value);
                break;
            }
            case '-': {
                solver.erase(value);
                break;
            }
        }
    }
    return 0;
}

void Set::rotation(Node* current, Rotations type) {
    Node* pivot = nullptr;
    if (type == Rotations::RIGHT) {
        pivot = current->left;
    } else {
        pivot = current->right;
    }

    if (this->root_ == current) {
        this->root_ = pivot;
        pivot->parent = nullptr;
    } else {
        if (current->isLeftChild()) {
            current->parent->left = pivot;
        } else {
            current->parent->right = pivot;
        }
        pivot->parent = current->parent;
    }

    if (type == Rotations::RIGHT) {
        current->left = pivot->right;
        if (pivot->right) {
            pivot->right->parent = current;
        }
        pivot->right = current;
        current->parent = pivot;
    } else {
        current->right = pivot->left;
        if (pivot->left) {
            pivot->left->parent = current;
        }
        pivot->left = current;
        current->parent = pivot;
    }
    current->recalculateSize();
    pivot->recalculateSize();
}

void Set::fixBalance(Node* current) {
    if (!current) {
        return;
    }

    if (this->root_ == current) {
        current->color = Node::Color::BLACK;
        current->recalculateSize();
        return;
    }
    if (current->color == Node::Color::BLACK || current->parent->color == Node::Color::BLACK) {
        this->recalculateSize(current->parent);
        return;
    }
    Node* parent = current->parent;      // definitely exists, definitely red
    Node* grandparent = parent->parent;  // definitely exists because parent is red (thus, not root)
    Node* uncle = parent->isLeftChild() ? grandparent->right : grandparent->left;  // may not exist
    if (uncle && uncle->color == Node::Color::RED) {
        // repaint
        grandparent->color = Node::Color::RED;
        parent->color = Node::Color::BLACK;
        if (uncle) {
            uncle->color = Node::Color::BLACK;
        }
    } else {
        // repaint + rotation
        if (parent->isLeftChild()) {
            if (current->isRightChild()) {
                this->rotation(parent, Rotations::LEFT);
                std::swap(parent, current);
            }
            parent->color = Node::Color::BLACK;
            grandparent->color = Node::Color::RED;
            this->rotation(grandparent, Rotations::RIGHT);
        } else {
            if (current->isLeftChild()) {
                this->rotation(parent, Rotations::RIGHT);
                std::swap(parent, current);
            }
            parent->color = Node::Color::BLACK;
            grandparent->color = Node::Color::RED;
            this->rotation(grandparent, Rotations::LEFT);
        }
    }
    this->fixBalance(parent);
    this->fixBalance(grandparent);
}

void Set::fixDoubleBlack(Node* current) {
    if (current == this->root_) {
        current->color = Node::Color::BLACK;
        current->is_double_black = false;
        return;
    }
    Node* parent = current->parent;                                         // definitely exists
    Node* sibling = current->isLeftChild() ? parent->right : parent->left;  // definitely exists
    if (sibling->color == Node::Color::BLACK &&
        (!sibling->left || sibling->left->color == Node::Color::BLACK) &&
        (!sibling->right || sibling->right->color == Node::Color::BLACK))  // Case 3
    {
        sibling->color = Node::Color::RED;
        current->is_double_black = false;
        if (parent->color == Node::Color::RED) {
            parent->color = Node::Color::BLACK;  // Case 3.1
        } else {
            parent->is_double_black = true;  // Case 3.2
        }
        if (parent->is_double_black) {
            this->fixDoubleBlack(parent);
        }
        return;
    }
    if (sibling->color == Node::Color::RED &&
        (!sibling->left || sibling->left->color == Node::Color::BLACK) &&
        (!sibling->right || sibling->right->color == Node::Color::BLACK))  // Case 4
    {
        this->rotation(parent, current->isLeftChild() ? Rotations::LEFT : Rotations::RIGHT);
        parent->color = Node::Color::RED;
        sibling->color = Node::Color::BLACK;
        this->fixDoubleBlack(current);
        return;
    }

    // sibling is definitely black
    if ((current->isLeftChild() && sibling->right && sibling->right->color == Node::Color::RED) ||
        (current->isRightChild() && sibling->left &&
         sibling->left->color == Node::Color::RED))  // Case 6
    {
        this->rotation(parent, current->isLeftChild() ? Rotations::LEFT : Rotations::RIGHT);
        sibling->color = parent->color;
        parent->color = Node::Color::BLACK;

        if (current->isLeftChild()) {
            sibling->right->color = Node::Color::BLACK;
        } else {
            sibling->left->color = Node::Color::BLACK;
        }
        current->is_double_black = false;
    } else  // Case 5 (sibling->left->color == Color::RED) || (sibling->right->color == Color::RED)
    {
        if (current->isLeftChild()) {
            sibling->left->color = Node::Color::BLACK;
        } else {
            sibling->right->color = Node::Color::BLACK;
        }
        sibling->color = Node::Color::RED;
        this->rotation(sibling, current->isLeftChild() ? Rotations::RIGHT : Rotations::LEFT);
        this->fixDoubleBlack(current);
    }
}

void Set::recalculateSize(Node* current) {
    if (!current) {
        return;
    }
    current->recalculateSize();
    this->recalculateSize(current->parent);
}

void Set::insert(int value) {
    if (!this->root_) {
        this->root_ = new Node(value);
        this->fixBalance(this->root_);
        return;
    }
    Node* current = this->root_;
    while (value != current->value) {
        if (value < current->value) {
            if (!current->left) {
                current->left = new Node(value, current);
                this->fixBalance(current->left);
                return;
            }
            current = current->left;
        } else {
            if (!current->right) {
                current->right = new Node(value, current);
                this->fixBalance(current->right);
                return;
            }
            current = current->right;
        }
    }
}

void Set::erase(int value) {
    Node* current = this->root_;
    while (current && value != current->value) {
        if (value < current->value) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    if (!current || value != current->value) {
        return;
    }
    if (current->left && current->right) {
        Node* iop = current->left;
        while (iop->right) {
            iop = iop->right;
        }
        current->value = iop->value;
        current = iop;
    }

    Node* parent = current->parent;
    if (current->left && !current->right) {
        if (!parent) {
            this->root_ = current->left;
            current->left->parent = nullptr;
            delete current;
            this->recalculateSize(this->root_);
            this->root_->color = Node::Color::BLACK;
            return;
        }
        if (current->isLeftChild()) {
            parent->left = current->left;
        } else {
            parent->right = current->left;
        }
        current->left->parent = parent;
        if (current->color == Node::Color::BLACK) {
            current->left->color = Node::Color::BLACK;
        }
        delete current;
        this->recalculateSize(parent);
        return;
    }
    if (!current->left && current->right) {
        if (!parent) {
            this->root_ = current->right;
            current->right->parent = nullptr;
            delete current;
            this->recalculateSize(this->root_);
            this->root_->color = Node::Color::BLACK;
            return;
        }
        if (current->isLeftChild()) {
            parent->left = current->right;
        } else {
            parent->right = current->right;
        }
        current->right->parent = parent;
        if (current->color == Node::Color::BLACK) {
            current->right->color = Node::Color::BLACK;
        }
        delete current;
        this->recalculateSize(parent);
        return;
    }
    if (!current->left && !current->right) {
        if (!parent) {
            this->root_ = nullptr;
            delete current;
            return;
        }
        if (current->color == Node::Color::RED) {
            if (current->isLeftChild()) {
                parent->left = nullptr;
            } else {
                parent->right = nullptr;
            }
            delete current;
            this->recalculateSize(parent);
            return;
        }
        current->is_double_black = true;
        this->fixDoubleBlack(current);
        if (current->isLeftChild()) {
            current->parent->left = nullptr;
        } else {
            current->parent->right = nullptr;
        }
        delete current;
        this->recalculateSize(parent);
    }
}

int Set::get(unsigned int index) {
    Node* current = this->root_;
    unsigned int left_size = current->left ? current->left->size : 0;
    while (left_size + 1 != index) {
        if (left_size >= index) {
            current = current->left;
            left_size = current->left ? current->left->size : 0;
        } else {
            current = current->right;
            index = index - left_size - 1;
            left_size = current->left ? current->left->size : 0;
        }
    }
    return current->value;
}