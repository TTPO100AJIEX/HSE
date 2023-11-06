#include <initializer_list>
#include <utility>

enum class Color { RED, BLACK };
template <typename ValueType>
class RBTree;

template <typename ValueType>
struct Node {
    friend class RBTree<ValueType>;

private:
    bool isLeftChild() const {
        return (this->parent ? (this->parent->left == this) : false);
    }
    bool isRightChild() const {
        return (this->parent ? (this->parent->right == this) : false);
    }

    Node(const ValueType& value, Node<ValueType>* parent) : value(value), parent(parent){};
    Node(const Node<ValueType>& other, Node<ValueType>* parent)
        : value(other.value), parent(parent), color(other.color) {
        if (other.left) {
            this->left = new Node<ValueType>(*other.left, this);
        }
        if (other.right) {
            this->right = new Node<ValueType>(*other.right, this);
        }
    }

    bool is_double_black_ = false;

public:
    ValueType value;
    Node<ValueType>* left = nullptr;
    Node<ValueType>* right = nullptr;
    Node<ValueType>* parent = nullptr;
    Color color = Color::RED;

    Node() = default;
    explicit Node(const ValueType& value) : value(value){};
};

template <typename ValueType>
class RBTree {
private:
    std::size_t length_ = 0;

    enum class Rotations { LEFT, RIGHT };
    void rotation(Node<ValueType>* root, Rotations type);
    void fixBalance(Node<ValueType>* current);
    void fixDoubleBlack(Node<ValueType>* current);

public:
    Node<ValueType>* root = nullptr;
    struct Iterator;

    RBTree() = default;
    RBTree(std::initializer_list<ValueType> list) {
        for (const ValueType& value : list) {
            this->insert(value);
        }
    }                                                              // O(nlog(n))
    RBTree(const RBTree<ValueType>& other);                        // O(n)
    RBTree<ValueType>& operator=(const RBTree<ValueType>& other);  // O(n)

    void destroy(Node<ValueType>* node) {
        if (!node) {
            return;
        }
        this->destroy(node->left);
        this->destroy(node->right);
        delete node;
    }
    ~RBTree() {
        this->destroy(this->root);
    }  // O(n)

    void insert(const ValueType& value);  // O(log(n))
    void erase(const ValueType& value);   // O(log(n))

    Iterator lowerBound(const ValueType& value) const;  // O(log(n))
    Iterator find(const ValueType& value) const;        // O(log(n))
    Iterator begin() const;                             // O(1) | NOW: O(logn)
    Iterator end() const;                               // O(1)

    std::size_t size() const {
        return (this->length_);
    }  // O(1)
    bool empty() const {
        return (!this->length_);
    }  // O(1)

    struct Iterator {
    private:
        Node<ValueType>* node_ = nullptr;

    public:
        Iterator() = default;
        explicit Iterator(Node<ValueType>* node) : node_(node) {
        }

        const ValueType& operator*() const {
            return this->node_->value;
        }
        const ValueType* operator->() const {
            return &(this->node_->value);
        }

        Iterator& operator++();
        Iterator operator++(int);

        Iterator& operator--();
        Iterator operator--(int);

        bool operator==(const Iterator& other) const {
            return this->node_ == other.node_;
        }
        bool operator!=(const Iterator& other) const {
            return this->node_ != other.node_;
        }
    };
};

template <typename ValueType>
RBTree<ValueType>::RBTree(const RBTree<ValueType>& other) {
    if (!other.root) {
        return;
    }
    this->root = new Node<ValueType>(*other.root, nullptr);
    this->length_ = other.length_;
}
template <typename ValueType>
RBTree<ValueType>& RBTree<ValueType>::operator=(const RBTree<ValueType>& other) {
    this->destroy(this->root);
    this->root = nullptr;
    this->length_ = 0;
    if (other.root) {
        this->root = new Node<ValueType>(*other.root, nullptr);
        this->length_ = other.length_;
    }
    return *this;
}

template <typename ValueType>
void RBTree<ValueType>::rotation(Node<ValueType>* current, RBTree<ValueType>::Rotations type) {
    Node<ValueType>* pivot = nullptr;
    if (type == RBTree<ValueType>::Rotations::RIGHT) {
        pivot = current->left;
    } else {
        pivot = current->right;
    }

    if (this->root == current) {
        this->root = pivot;
        pivot->parent = nullptr;
    } else {
        if (current->isLeftChild()) {
            current->parent->left = pivot;
        } else {
            current->parent->right = pivot;
        }
        pivot->parent = current->parent;
    }

    if (type == RBTree<ValueType>::Rotations::RIGHT) {
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
}
template <typename ValueType>
void RBTree<ValueType>::fixBalance(Node<ValueType>* current) {
    if (this->root == current) {
        current->color = Color::BLACK;
        return;
    }
    if (!current || current->color == Color::BLACK || current->parent->color == Color::BLACK) {
        return;
    }
    Node<ValueType>* parent = current->parent;  // definitely exists, definitely red
    Node<ValueType>* grandparent =
        parent->parent;  // definitely exists because parent is red (thus, not root)
    Node<ValueType>* uncle =
        parent->isLeftChild() ? grandparent->right : grandparent->left;  // may not exist
    if (uncle && uncle->color == Color::RED) {
        // repaint
        grandparent->color = Color::RED;
        parent->color = Color::BLACK;
        if (uncle) {
            uncle->color = Color::BLACK;
        }
    } else {
        // repaint + rotation
        if (parent->isLeftChild()) {
            if (current->isRightChild()) {
                this->rotation(parent, RBTree<ValueType>::Rotations::LEFT);
                std::swap(parent, current);
            }
            parent->color = Color::BLACK;
            grandparent->color = Color::RED;
            this->rotation(grandparent, RBTree<ValueType>::Rotations::RIGHT);
        } else {
            if (current->isLeftChild()) {
                this->rotation(parent, RBTree<ValueType>::Rotations::RIGHT);
                std::swap(parent, current);
            }
            parent->color = Color::BLACK;
            grandparent->color = Color::RED;
            this->rotation(grandparent, RBTree<ValueType>::Rotations::LEFT);
        }
    }
    this->fixBalance(parent);
    this->fixBalance(grandparent);
}

template <typename ValueType>
void RBTree<ValueType>::insert(const ValueType& value) {
    if (this->length_ == 0) {
        this->root = new Node<ValueType>(value);
        ++this->length_;
        this->fixBalance(this->root);
        return;
    }
    Node<ValueType>* current = this->root;
    while (value < current->value || current->value < value)  // value != current->value
    {
        if (value < current->value) {
            if (!current->left) {
                current->left = new Node<ValueType>(value, current);
                ++this->length_;
                this->fixBalance(current->left);
                return;
            }
            current = current->left;
        } else {
            if (!current->right) {
                current->right = new Node<ValueType>(value, current);
                ++this->length_;
                this->fixBalance(current->right);
                return;
            }
            current = current->right;
        }
    }
}

template <typename ValueType>
void RBTree<ValueType>::fixDoubleBlack(Node<ValueType>* current) {
    if (current == this->root) {
        current->color = Color::BLACK;
        current->is_double_black_ = false;
        return;
    }
    Node<ValueType>* parent = current->parent;  // definitely exists
    Node<ValueType>* sibling =
        current->isLeftChild() ? parent->right : parent->left;  // definitely exists
    if (sibling->color == Color::BLACK &&
        (!sibling->left || sibling->left->color == Color::BLACK) &&
        (!sibling->right || sibling->right->color == Color::BLACK))  // Case 3
    {
        sibling->color = Color::RED;
        current->is_double_black_ = false;
        if (parent->color == Color::RED) {
            parent->color = Color::BLACK;  // Case 3.1
        } else {
            parent->is_double_black_ = true;  // Case 3.2
        }
        if (parent->is_double_black_) {
            this->fixDoubleBlack(parent);
        }
        return;
    }
    if (sibling->color == Color::RED && (!sibling->left || sibling->left->color == Color::BLACK) &&
        (!sibling->right || sibling->right->color == Color::BLACK))  // Case 4
    {
        this->rotation(parent, current->isLeftChild() ? RBTree<ValueType>::Rotations::LEFT
                                                      : RBTree<ValueType>::Rotations::RIGHT);
        parent->color = Color::RED;
        sibling->color = Color::BLACK;
        this->fixDoubleBlack(current);
        return;
    }

    // sibling is definitely black
    if ((current->isLeftChild() && sibling->right && sibling->right->color == Color::RED) ||
        (current->isRightChild() && sibling->left && sibling->left->color == Color::RED))  // Case 6
    {
        this->rotation(parent, current->isLeftChild() ? RBTree<ValueType>::Rotations::LEFT
                                                      : RBTree<ValueType>::Rotations::RIGHT);
        sibling->color = parent->color;
        parent->color = Color::BLACK;

        if (current->isLeftChild()) {
            sibling->right->color = Color::BLACK;
        } else {
            sibling->left->color = Color::BLACK;
        }
        current->is_double_black_ = false;
    } else  // Case 5 (sibling->left->color == Color::RED) || (sibling->right->color == Color::RED)
    {
        if (current->isLeftChild()) {
            sibling->left->color = Color::BLACK;
        } else {
            sibling->right->color = Color::BLACK;
        }
        sibling->color = Color::RED;
        this->rotation(sibling, current->isLeftChild() ? RBTree<ValueType>::Rotations::RIGHT
                                                       : RBTree<ValueType>::Rotations::LEFT);
        this->fixDoubleBlack(current);
    }
}

template <typename ValueType>
void RBTree<ValueType>::erase(const ValueType& value) {
    Node<ValueType>* current = this->root;
    while (current && (value < current->value ||
                       current->value < value))  // current && value != current->value
    {
        if (value < current->value) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    if (!current || value < current->value || current->value < value) {
        return;  // !current || value != current->value
    }
    this->length_--;
    if (current->left && current->right) {
        Node<ValueType>* iop = current->left;
        while (iop->right) {
            iop = iop->right;
        }
        current->value = iop->value;
        current = iop;
    }

    Node<ValueType>* parent = current->parent;
    if (current->left && !current->right) {
        if (!parent) {
            this->root = current->left;
            current->left->parent = nullptr;
            delete current;
            this->root->color = Color::BLACK;
            return;
        }
        if (current->isLeftChild()) {
            parent->left = current->left;
        } else {
            parent->right = current->left;
        }
        current->left->parent = parent;
        if (current->color == Color::BLACK) {
            current->left->color = Color::BLACK;
        }
        delete current;
        return;
    }
    if (!current->left && current->right) {
        if (!parent) {
            this->root = current->right;
            current->right->parent = nullptr;
            delete current;
            this->root->color = Color::BLACK;
            return;
        }
        if (current->isLeftChild()) {
            parent->left = current->right;
        } else {
            parent->right = current->right;
        }
        current->right->parent = parent;
        if (current->color == Color::BLACK) {
            current->right->color = Color::BLACK;
        }
        delete current;
        return;
    }
    if (!current->left && !current->right) {
        if (!parent) {
            this->root = nullptr;
            delete current;
            return;
        }
        if (current->color == Color::RED) {
            if (current->isLeftChild()) {
                parent->left = nullptr;
            } else {
                parent->right = nullptr;
            }
            delete current;
            return;
        }
        current->is_double_black_ = true;
        this->fixDoubleBlack(current);
        if (current->isLeftChild()) {
            current->parent->left = nullptr;
        } else {
            current->parent->right = nullptr;
        }
        delete current;
    }
}

template <typename ValueType>
typename RBTree<ValueType>::Iterator RBTree<ValueType>::find(const ValueType& value) const {
    Node<ValueType>* current = this->root;
    while (current && (value < current->value ||
                       current->value < value))  // current && value != current->value
    {
        if (value < current->value) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return RBTree<ValueType>::Iterator(current);
}
template <typename ValueType>
typename RBTree<ValueType>::Iterator RBTree<ValueType>::lowerBound(const ValueType& value) const {
    Node<ValueType>* answer = nullptr;
    Node<ValueType>* current = this->root;
    while (current) {
        if (!(value < current->value || current->value < value)) {
            return RBTree<ValueType>::Iterator(current);  // value == current->value
        }
        if (value < current->value) {
            answer = current;
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return RBTree<ValueType>::Iterator(answer);
}
template <typename ValueType>
typename RBTree<ValueType>::Iterator RBTree<ValueType>::begin() const {
    Node<ValueType>* current = this->root;
    if (!current) {
        return RBTree<ValueType>::Iterator(nullptr);
    }
    while (current->left) {
        current = current->left;
    }
    return RBTree<ValueType>::Iterator(current);
}
template <typename ValueType>
typename RBTree<ValueType>::Iterator RBTree<ValueType>::end() const {
    return RBTree<ValueType>::Iterator(nullptr);
}

template <typename ValueType>
typename RBTree<ValueType>::Iterator& RBTree<ValueType>::Iterator::operator++() {
    if (this->node_->right) {
        this->node_ = this->node_->right;
        while (this->node_->left) {
            this->node_ = this->node_->left;
        }
    } else {
        while (this->node_->isRightChild()) {
            this->node_ = this->node_->parent;
        }
        this->node_ = this->node_->parent;
    }
    return *this;
}
template <typename ValueType>
typename RBTree<ValueType>::Iterator RBTree<ValueType>::Iterator::operator++(int) {
    RBTree<ValueType>::Iterator save = *this;
    ++(*this);
    return save;
}

template <typename ValueType>
typename RBTree<ValueType>::Iterator& RBTree<ValueType>::Iterator::operator--() {
    if (!this->node_) {
        this->node_ = this->root;
        while (this->node_->right) {
            this->node_ = this->node_->right;
        }
        return *this;
    }
    if (this->node_->left) {
        this->node_ = this->node_->left;
    } else {
        while (this->node_->isLeftChild()) {
            this->node_ = this->node_->parent;
        }
        this->node_ = this->node_->parent;
    }
    return *this;
}
template <typename ValueType>
typename RBTree<ValueType>::Iterator RBTree<ValueType>::Iterator::operator--(int) {
    RBTree<ValueType>::Iterator save = *this;
    --(*this);
    return save;
}