#include <initializer_list>
#include <utility>

enum class Color { RED, BLACK };
template <typename K, typename V>
class Map;

template <typename K, typename V>
struct Node {
    friend class Map<K, V>;

private:
    bool isLeftChild() const {
        return (this->parent ? (this->parent->left == this) : false);
    }
    bool isRightChild() const {
        return (this->parent ? (this->parent->right == this) : false);
    }

    Node(const K& key, const V& value, Node<K, V>* parent)
        : data_({key, value}), key(key), value(value), parent(parent){};
    Node(const Node<K, V>& other, Node<K, V>* parent)
        : data_({other.key, other.value}),
          key(other.key),
          value(other.value),
          parent(parent),
          color(other.color) {
        if (other.left) {
            this->left = new Node<K, V>(*other.left, this);
        }
        if (other.right) {
            this->right = new Node<K, V>(*other.right, this);
        }
    }

    bool is_double_black_ = false;
    std::pair<K, V> data_;

public:
    K key;
    V value;
    Node<K, V>* left = nullptr;
    Node<K, V>* right = nullptr;
    Node<K, V>* parent = nullptr;
    Color color = Color::RED;

    Node() = default;
    Node(const K& key, const V& value) : data_({key, value}), key(key), value(value){};
};

template <typename K, typename V>
class Map {
private:
    std::size_t length_ = 0;

    enum class Rotations { LEFT, RIGHT };
    void rotation(Node<K, V>* root, Rotations type);
    void fixBalance(Node<K, V>* current);
    void fixDoubleBlack(Node<K, V>* current);

public:
    Node<K, V>* root = nullptr;
    struct Iterator;

    Map() = default;
    Map(std::initializer_list<std::pair<K, V> > list) {
        for (const std::pair<K, V>& data : list) {
            this->insert(data.first, data.second);
        }
    }                                              // O(nlog(n))
    Map(const Map<K, V>& other);                   // O(n)
    Map<K, V>& operator=(const Map<K, V>& other);  // O(n)

    void destroy(Node<K, V>* node) {
        if (!node) {
            return;
        }
        this->destroy(node->left);
        this->destroy(node->right);
        delete node;
    }
    ~Map() {
        this->destroy(this->root);
    }  // O(n)

    void insert(const K& key, const V& value);  // O(log(n))
    void erase(const K& key);                   // O(log(n))

    Iterator lowerBound(const K& key) const;  // O(log(n))
    Iterator find(const K& key) const;        // O(log(n))
    Iterator begin() const;                   // O(1) | NOW: O(logn)
    Iterator end() const;                     // O(1)

    std::size_t size() const {
        return (this->length_);
    }  // O(1)
    bool empty() const {
        return (!this->length_);
    }  // O(1)

    struct Iterator {
    private:
        Node<K, V>* node_ = nullptr;

    public:
        Iterator() = default;
        explicit Iterator(Node<K, V>* node) : node_(node) {
        }

        const std::pair<K, V>& operator*() const {
            return this->node_->data_;
        }
        const std::pair<K, V>* operator->() const {
            return &(this->node_->data_);
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

template <typename K, typename V>
Map<K, V>::Map(const Map<K, V>& other) {
    if (!other.root) {
        return;
    }
    this->root = new Node<K, V>(*other.root, nullptr);
    this->length_ = other.length_;
}
template <typename K, typename V>
Map<K, V>& Map<K, V>::operator=(const Map<K, V>& other) {
    this->destroy(this->root);
    this->root = nullptr;
    this->length_ = 0;
    if (other.root) {
        this->root = new Node<K, V>(*other.root, nullptr);
        this->length_ = other.length_;
    }
    return *this;
}

template <typename K, typename V>
void Map<K, V>::rotation(Node<K, V>* current, Rotations type) {
    Node<K, V>* pivot = nullptr;
    if (type == Rotations::RIGHT) {
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
}
template <typename K, typename V>
void Map<K, V>::fixBalance(Node<K, V>* current) {
    if (this->root == current) {
        current->color = Color::BLACK;
        return;
    }
    if (!current || current->color == Color::BLACK || current->parent->color == Color::BLACK) {
        return;
    }
    Node<K, V>* parent = current->parent;  // definitely exists, definitely red
    Node<K, V>* grandparent =
        parent->parent;  // definitely exists because parent is red (thus, not root)
    Node<K, V>* uncle =
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
                this->rotation(parent, Rotations::LEFT);
                std::swap(parent, current);
            }
            parent->color = Color::BLACK;
            grandparent->color = Color::RED;
            this->rotation(grandparent, Rotations::RIGHT);
        } else {
            if (current->isLeftChild()) {
                this->rotation(parent, Rotations::RIGHT);
                std::swap(parent, current);
            }
            parent->color = Color::BLACK;
            grandparent->color = Color::RED;
            this->rotation(grandparent, Rotations::LEFT);
        }
    }
    this->fixBalance(parent);
    this->fixBalance(grandparent);
}

template <typename K, typename V>
void Map<K, V>::insert(const K& key, const V& value) {
    if (this->length_ == 0) {
        this->root = new Node<K, V>(key, value);
        ++this->length_;
        this->fixBalance(this->root);
        return;
    }
    Node<K, V>* current = this->root;
    while (key < current->key || current->key < key)  // key != current->key
    {
        if (key < current->key) {
            if (!current->left) {
                current->left = new Node<K, V>(key, value, current);
                ++this->length_;
                this->fixBalance(current->left);
                return;
            }
            current = current->left;
        } else {
            if (!current->right) {
                current->right = new Node<K, V>(key, value, current);
                ++this->length_;
                this->fixBalance(current->right);
                return;
            }
            current = current->right;
        }
    }
    current->value = value;
    current->data_.second = value;
}

template <typename K, typename V>
void Map<K, V>::fixDoubleBlack(Node<K, V>* current) {
    if (current == this->root) {
        current->color = Color::BLACK;
        current->is_double_black_ = false;
        return;
    }
    Node<K, V>* parent = current->parent;  // definitely exists
    Node<K, V>* sibling =
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
        this->rotation(parent, current->isLeftChild() ? Rotations::LEFT : Rotations::RIGHT);
        parent->color = Color::RED;
        sibling->color = Color::BLACK;
        this->fixDoubleBlack(current);
        return;
    }

    // sibling is definitely black
    if ((current->isLeftChild() && sibling->right && sibling->right->color == Color::RED) ||
        (current->isRightChild() && sibling->left && sibling->left->color == Color::RED))  // Case 6
    {
        this->rotation(parent, current->isLeftChild() ? Rotations::LEFT : Rotations::RIGHT);
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
        this->rotation(sibling, current->isLeftChild() ? Rotations::RIGHT : Rotations::LEFT);
        this->fixDoubleBlack(current);
    }
}

template <typename K, typename V>
void Map<K, V>::erase(const K& key) {
    Node<K, V>* current = this->root;
    while (current && (key < current->key || current->key < key))  // current && key != current->key
    {
        if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    if (!current || key < current->key || current->key < key) {
        return;  // !current || key != current->key
    }
    this->length_--;
    if (current->left && current->right) {
        Node<K, V>* iop = current->left;
        while (iop->right) {
            iop = iop->right;
        }
        current->key = iop->key;
        current->value = iop->value;
        current->data_ = iop->data_;
        current = iop;
    }

    Node<K, V>* parent = current->parent;
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

template <typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::find(const K& key) const {
    Node<K, V>* current = this->root;
    while (current && (key < current->key || current->key < key))  // current && key != current->key
    {
        if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return Map<K, V>::Iterator(current);
}
template <typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::lowerBound(const K& key) const {
    Node<K, V>* answer = nullptr;
    Node<K, V>* current = this->root;
    while (current) {
        if (!(key < current->key || current->key < key)) {
            return Map<K, V>::Iterator(current);  // key == current->key
        }
        if (key < current->key) {
            answer = current;
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return Map<K, V>::Iterator(answer);
}
template <typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::begin() const {
    Node<K, V>* current = this->root;
    if (!current) {
        return Map<K, V>::Iterator(nullptr);
    }
    while (current->left) {
        current = current->left;
    }
    return Map<K, V>::Iterator(current);
}
template <typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::end() const {
    return Map<K, V>::Iterator(nullptr);
}

template <typename K, typename V>
typename Map<K, V>::Iterator& Map<K, V>::Iterator::operator++() {
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
template <typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::Iterator::operator++(int) {
    Map<K, V>::Iterator save = *this;
    ++(*this);
    return save;
}

template <typename K, typename V>
typename Map<K, V>::Iterator& Map<K, V>::Iterator::operator--() {
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
template <typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::Iterator::operator--(int) {
    Map<K, V>::Iterator save = *this;
    --(*this);
    return save;
}