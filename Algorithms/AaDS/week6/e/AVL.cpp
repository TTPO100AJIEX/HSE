#include <iostream>
#include <utility>

class Set
{
    struct Node
    {
        bool isLeftChild() const { return (this->parent ? (this->parent->left == this) : false); }
        bool isRightChild() const { return (this->parent ? (this->parent->right == this) : false); }
        
        void recalculateHeight()
        {
            unsigned int left_height = (this->left ? this->left->height : 0), right_height = (this->right ? this->right->height : 0);
            this->height = 1 + (left_height > right_height ? left_height : right_height);
            
            unsigned int left_size = (this->left ? this->left->size : 0), right_size = (this->right ? this->right->size : 0);
            this->size = 1 + left_size + right_size;
        }
        int balance() const { return ((this->left ? this->left->height : 0) - (this->right ? this->right->height : 0)); }

        unsigned int height = 1;
        unsigned int size = 1;
        Node* left = nullptr;
        Node* right = nullptr;
        Node* parent = nullptr;
        int value = 0;

        Node(int value) : value(value) { }
        Node(int value, Node* parent) : parent(parent), value(value) { }
    };
    Node* root = nullptr;
    
    enum class Rotations { LEFT, RIGHT };
    Node* rotation(Node* root, Rotations type);
    void fix_balance(Node* root);

public:
    int get(unsigned int index);
    void insert(int value);
    void erase(int value);
    
    void destroy(Node* node)
    {
        if (!node) return;
        this->destroy(node->left);
        this->destroy(node->right);
        delete node;
    }
    ~Set() { this->destroy(this->root); }

    /*void output(Node* now, std::string path)
    {
        if (!now) return;
        this->output(now->left, path + "l");
        this->output(now->right, path + "r");
        std::cout << now->value << " " << path << " " << now->height << " " << now->size << std::endl;
    }
    Node* GetRoot() { return this->root; }*/
};

int main(void)
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n;
    std::cin >> n;
    Set solver;
    for (unsigned int i = 0; i < n; ++i)
    {
        char type;
        int value;
        std::cin >> type >> value;
        switch (type)
        {
            case '?': { std::cout << solver.get(value) << '\n'; break; }
            case '+': { solver.insert(value); break; }
            case '-': { solver.erase(value); break; }
        }
        //solver.output(solver.GetRoot(), "");
    }
    return 0;
}



Set::Node* Set::rotation(Node* current, Rotations type)
{
    Node* pivot = nullptr;
    if (type == Rotations::RIGHT) pivot = current->left;
    else pivot = current->right;

    if (this->root == current) { this->root = pivot; pivot->parent = nullptr; }
    else
    {
        if (current->isLeftChild()) current->parent->left = pivot;
        else current->parent->right = pivot;
        pivot->parent = current->parent;
    }

    if (type == Rotations::RIGHT)
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
    current->recalculateHeight();
    pivot->recalculateHeight();
    if (pivot->parent) pivot->parent->recalculateHeight();
    return pivot;
}


void Set::fix_balance(Set::Node* current)
{
    if (!current) return;
    current->recalculateHeight();
    if (current->left) current->left->recalculateHeight();
    if (current->right) current->right->recalculateHeight();
    if (current->balance() > 1)
    {
        // too many vertexes on the left
        if (current->left->balance() < 0) this->rotation(current->left, Rotations::LEFT);
        current = this->rotation(current, Rotations::RIGHT);
        this->fix_balance(current);
        return;
    }
    if (current->balance() < -1)
    {
        // too many vertexes on the right
        if (current->right->balance() > 0) this->rotation(current->right, Rotations::RIGHT);
        current = this->rotation(current, Rotations::LEFT);
        this->fix_balance(current);
        return;
    }
    this->fix_balance(current->parent);
}



void Set::insert(int value)
{
    if (!this->root) { this->root = new Node(value); return; }
    Node* current = this->root;
    while (value != current->value)
    {
        if (value < current->value)
        {
            if (!current->left) { current->left = new Node(value, current); this->fix_balance(current->left); return; }
            current = current->left;
        }
        else
        {
            if (!current->right) { current->right = new Node(value, current); this->fix_balance(current->right); return; }
            current = current->right;
        }
    }
}



void Set::erase(int value)
{
    Node* current = this->root;
    while (current && value != current->value)
    {
        if (value < current->value) current = current->left;
        else current = current->right;
    }
    if (!current || value != current->value) return;
    if (current->left && current->right)
    {
        Node* iop = current->left;
        while (iop->right) iop = iop->right;
        current->value = iop->value;
        current = iop;
    }
    
    Node* parent = nullptr;
    if (!current->left && !current->right)
    {
        parent = current->parent;
        if (!parent) { this->root = nullptr; delete current; return; }
        if (current->isLeftChild()) parent->left = nullptr;
        else parent->right = nullptr;
        delete current;
        this->fix_balance(parent);
        return;
    }
    if (current->left && !current->right)
    {
        parent = current->parent;
        if (!parent) { this->root = current->left; current->left->parent = nullptr; delete current; return; }
        if (current->isLeftChild()) parent->left = current->left;
        else parent->right = current->left;
        current->left->parent = parent;
        delete current;
        this->fix_balance(parent);
        return;
    }
    if (!current->left && current->right)
    {
        parent = current->parent;
        if (!parent) { this->root = current->right; current->right->parent = nullptr; delete current; return; }
        if (current->isLeftChild()) parent->left = current->right;
        else parent->right = current->right;
        current->right->parent = parent;
        delete current;
        this->fix_balance(parent);
        return;
    }
}



int Set::get(unsigned int index)
{
    Node* current = this->root;
    unsigned int left_size = current->left ? current->left->size : 0;
    while (left_size + 1 != index)
    {
        if (left_size >= index)
        {
            current = current->left;
            left_size = current->left ? current->left->size : 0;
        }
        else
        {
            current = current->right;
            index = index - left_size - 1;
            left_size = current->left ? current->left->size : 0;
        }
    }
    return current->value;
}