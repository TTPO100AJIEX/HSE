#include <stdexcept>

class Node {
public:
    int data = 0;
    Node* next = nullptr;
    Node* previous = nullptr;

    Node() = default;
    Node(int data);
};
Node::Node(int data) {
    if (data < -2e9 || data > 2e9) {
        throw std::runtime_error("Wrong Value!");
}
    this->data = data;
}

class List {
public:
    List() = default;
    List(int* values, size_t size);
    ~List();

    int pop();
    int pop(size_t position);
    int popExplicit(Node* node);

    void pushBack(int value);
    void pushFront(int value);
    void push(int value, size_t position);
    void pushExplicit(Node* new_node, Node* push_after);

    Node* head = nullptr;
    Node* tail = nullptr;
};

List::List(int* values, size_t size) {
    /*for (size_t i = 0; i < size; i++)
    {
        if (values[i] < -2e9 || values[i] > 2e9) throw std::runtime_error("Wrong Value!");
    }*/
    for (size_t i = 0; i < size; i++) {
        try {
            this->pushBack(values[i]);
        } catch (const std::exception& exception) {
            while (this->head != nullptr) {
                this->popExplicit(this->head);
}
            throw exception;
        }
    }
}

List::~List() {
<<<<<<< HEAD
    while (this->head != nullptr) {
        this->popExplicit(this->head);
}
=======
    Node* head = this->head;
    if (head == nullptr) {
        return;
    }
    do {
        Node* next = head->next;
        delete head;
        head = next;
    } while (head != this->head);
>>>>>>> 96684a92506886c1378a3f3c69268f757dccc0c4
}

int List::pop() {
    return this->popExplicit(this->head);
}
int List::pop(size_t position) {
    if (this->head == nullptr) {
        throw std::runtime_error("Wrong Position!");
}
    Node* node = this->head;
    for (; position != 0; position--) {
        node = node->next;
        if (node == this->head) {
            throw std::runtime_error("Wrong Position!");
}
    }
    node = node->next;
    if (node == this->head || node == nullptr) {
        throw std::runtime_error("Wrong Position!");
}
    return this->popExplicit(node);
}
int List::popExplicit(Node* node) {
    if (node == nullptr) {
        throw std::runtime_error("Can not pop such element!");
}
    node->next->previous = node->previous;
    node->previous->next = node->next;
    if (node == this->head && node == this->tail) {
        this->head = this->tail = nullptr;
    } else {
        if (node == this->head) {
            this->head = node->next;
}
        if (node == this->tail) {
            this->tail = node->previous;
}
    }
    int value = node->data;
    delete node;
    return value;
}

void List::pushBack(int value) {
    Node* new_node = new Node(value);
    this->pushExplicit(new_node, this->tail);
}
void List::pushFront(int value) {
    Node* new_node = new Node(value);
    this->pushExplicit(new_node, nullptr);
}
void List::push(int value, size_t position) {
    Node* new_node = new Node(value);
    Node* node = this->head;
    for (; position != 0; position--) {
        node = node->next;
        if (node == this->head) {
            delete new_node;
            throw std::runtime_error("Wrong Position!");
        }
    }
    if (node == nullptr) {
        delete new_node;
        throw std::runtime_error("Wrong Position!");
    }
    this->pushExplicit(new_node, node);
}
void List::pushExplicit(Node* new_node, Node* push_after) {
    if (push_after == nullptr && this->head == nullptr) {
        new_node->next = new_node->previous = new_node;
        this->head = this->tail = new_node;
        return;
    }
    if (push_after == this->tail) {
        this->tail = new_node;
    }
    if (push_after == nullptr) {
        push_after = this->tail;
        this->head = new_node;
    }
    new_node->previous = push_after;
    new_node->next = push_after->next;
    push_after->next->previous = new_node;
    push_after->next = new_node;
<<<<<<< HEAD
    if (push_after == this->tail) {
        this->tail = new_node;
}
=======
>>>>>>> 96684a92506886c1378a3f3c69268f757dccc0c4
}