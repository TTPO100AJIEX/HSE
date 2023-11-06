#include <initializer_list>
#include <utility>

enum class Color { RED, BLACK };
template <typename T> class RBTree;

template <typename T>
struct Node
{
    friend class RBTree<T>;
private:
    bool isLeftChild() const { return (this->parent ? (this->parent->left == this) : false); }
    bool isRightChild() const { return (this->parent ? (this->parent->right == this) : false); }

public:
    T key;
    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;
    Color color = Color::RED;
    
    Node(T key) : key(key) { };
    Node(T key, Node* parent) : key(key), parent(parent) { };
    ~Node() { delete this->left; delete this->right; }
};

template <typename T>
class RBTree
{
private:
    unsigned int length = 0;

    enum class Rotations { LEFT, RIGHT };
    void rotation(Node<T>* root, Rotations type);
    void fix_balance(Node<T>* current);

public:
    Node<T>* root = nullptr;

    RBTree() = default;
    RBTree(std::initializer_list<T> list) { for (const T& key : list) this->insert(key); } // O(nlog(n))
    ~RBTree() { delete this->root; } // O(n)

    void insert(const T& key); // O(log(n))

    T* find(const T& key) const; // O(log(n))
    T* lowerBound(const T& key) const; // O(log(n))

    unsigned int size() const { return(this->length); } // O(1)
    bool empty() const { return (!this->length); } // O(1)
};

template <typename T>
void RBTree<T>::rotation(Node<T>* current, RBTree<T>::Rotations type)
{
    Node<T>* pivot = nullptr;
    if (type == RBTree<T>::Rotations::RIGHT) pivot = current->left;
    else pivot = current->right;

    if (this->root == current) { this->root = pivot; pivot->parent = nullptr; }
    else
    {
        if (current->isLeftChild()) current->parent->left = pivot;
        else current->parent->right = pivot;
        pivot->parent = current->parent;
    }

    if (type == RBTree<T>::Rotations::RIGHT)
    {
        current->left = pivot->right;
        if (pivot->right) pivot->right->parent = current;
        pivot->right = current;
        current->parent = pivot;
    }
    else
    {
        current->right = pivot->left;
        if (pivot->left) pivot->left->parent = current;
        pivot->left = current;
        current->parent = pivot;
    }
}
template <typename T>
void RBTree<T>::fix_balance(Node<T>* current)
{
    if (this->root == current) { current->color = Color::BLACK; return; }
    if (!current || current->color == Color::BLACK || current->parent->color == Color::BLACK) return;
    Node<T>* parent = current->parent; // definitely exists, definitely red
    Node<T>* grandparent = parent->parent; // definitely exists because parent is red (thus, not root)
    Node<T>* uncle = parent->isLeftChild() ? grandparent->right : grandparent->left; // may not exist
    if (uncle && uncle->color == Color::RED)
    {
        // repaint
        grandparent->color = Color::RED;
        parent->color = Color::BLACK;
        if (uncle) uncle->color = Color::BLACK;
    }
    else
    {
        // repaint + rotation
        if (parent->isLeftChild())
        {
            if (current->isRightChild())
            {
                this->rotation(parent, RBTree<T>::Rotations::LEFT);
                std::swap(parent, current);
            }
            parent->color = Color::BLACK;
            grandparent->color = Color::RED;
            this->rotation(grandparent, RBTree<T>::Rotations::RIGHT);
        }
        else
        {
            if (current->isLeftChild())
            {
                this->rotation(parent, RBTree<T>::Rotations::RIGHT);
                std::swap(parent, current);
            }
            parent->color = Color::BLACK;
            grandparent->color = Color::RED;
            this->rotation(grandparent, RBTree<T>::Rotations::LEFT);
        }
    }
    this->fix_balance(parent);
    this->fix_balance(grandparent);
}

template <typename T>
void RBTree<T>::insert(const T&  key)
{
    if (this->length == 0) { this->root = new Node<T>(key); this->length++; this->fix_balance(this->root); return; }
    Node<T>* current = this->root;
    while (key != current->key)
    {
        if (key < current->key)
        {
            if (!current->left) { current->left = new Node<T>(key, current); this->length++; this->fix_balance(current->left); return; }
            current = current->left;
        }
        else
        {
            if (!current->right) { current->right = new Node<T>(key, current); this->length++; this->fix_balance(current->right); return; }
            current = current->right;
        }
    }
}

template <typename T>
T* RBTree<T>::find(const T& key) const
{
    Node<T>* current = this->root;
    while (current && key != current->key)
    {
        if (key < current->key) current = current->left;
        else current = current->right;
    }
    return (current ? &(current->key) : nullptr);
}
template <typename T>
T* RBTree<T>::lowerBound(const T& key) const
{
    Node<T>* answer = nullptr; Node<T>* current = this->root;
    while (current)
    {
        if (key == current->key) return &(current->key); 
        if (key < current->key) { answer = current; current = current->left; }
        else current = current->right;
    }
    return (answer ? &(answer->key) : nullptr);
}