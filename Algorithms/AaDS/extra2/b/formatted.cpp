#include <functional>
#include <cstdint>
#include <cstdlib>
#include <algorithm>
#include <stdexcept>

template <class KeyType, class ValueType>
struct Node {
    KeyType key;
    ValueType value;
    Node* next = nullptr;

    Node(KeyType key, ValueType value) : key(key), value(value) {
    }
    ~Node() {
        delete this->next;
    }
};

template <class KeyType, class ValueType, class Func = std::hash<KeyType>>
class HashTable {
private:
    Node<KeyType, ValueType>** data_;
    size_t allocated_, length_ = 0;
    double fill_factor_ = 0.5;
    Func hasher_;

    void rehash(Node<KeyType, ValueType>* current);
    void resize();

    void init(size_t initial_size = 100) {
        this->allocated_ = initial_size;
        this->data_ = static_cast<Node<KeyType, ValueType>**>(
            std::calloc(this->allocated_, sizeof(Node<KeyType, ValueType>*)));
    }

public:
    HashTable() {
        this->init();
    }
    explicit HashTable(Func func) : hasher_(func) {
        this->init();
    }
    HashTable(size_t max_size, double coeff, Func func = std::hash<KeyType>())
        : fill_factor_((coeff <= 0 || coeff > 1) ? 0.5 : coeff), hasher_(func) {
        this->init(max_size);
    }
    ~HashTable() {
        for (size_t i = 0; i < this->allocated_; ++i) {
            delete this->data_[i];
        }
        std::free(this->data_);
    }

    void insert(KeyType key, ValueType data);
    void erase(KeyType key);

    ValueType* find(KeyType key);
    Node<KeyType, ValueType>& operator[](uint64_t index) const;
    Node<KeyType, ValueType> at(uint64_t index) const;

    size_t size() const {
        return this->length_;
    }
    size_t capacity() const {
        return this->allocated_;
    }
};

template <class KeyType, class ValueType, class Func>
void HashTable<KeyType, ValueType, Func>::rehash(Node<KeyType, ValueType>* current) {
    if (!current) {
        return;
    }
    this->rehash(current->next);
    this->insert(current->key, current->value);
}

template <class KeyType, class ValueType, class Func>
void HashTable<KeyType, ValueType, Func>::resize() {
    if (this->size() <= this->fill_factor_ * this->capacity()) {
        return;
    }
    Node<KeyType, ValueType>** save = this->data_;
    this->length_ = 0;
    this->allocated_ <<= 1;
    this->data_ = static_cast<Node<KeyType, ValueType>**>(
        std::calloc(this->allocated_, sizeof(Node<KeyType, ValueType>*)));
    for (size_t i = 0; i < (this->allocated_ >> 1); ++i) {
        this->rehash(save[i]);
        delete save[i];
    }
    std::free(save);
}

template <class KeyType, class ValueType, class Func>
void HashTable<KeyType, ValueType, Func>::insert(KeyType key, ValueType data) {
    size_t index = this->hasher_(key) % this->allocated_;
    if (!this->data_[index]) {
        this->data_[index] = new Node<KeyType, ValueType>(key, data);
        ++this->length_;
    } else {
        Node<KeyType, ValueType>* current = this->data_[index];
        while (current->next && current->key != key) {
            current = current->next;
        }
        if (current->key == key) {
            current->value = data;
        } else {
            current->next = new Node<KeyType, ValueType>(key, data);
            ++this->length_;
        }
    }
    this->resize();
}

template <class KeyType, class ValueType, class Func>
ValueType* HashTable<KeyType, ValueType, Func>::find(KeyType key) {
    if (this->allocated_ == 0) {
        return nullptr;
    }
    size_t index = this->hasher_(key) % this->allocated_;
    Node<KeyType, ValueType>* current = this->data_[index];
    while (current && current->key != key) {
        current = current->next;
    }
    if (!current) {
        return nullptr;
    }
    return &(current->value);
}

template <class KeyType, class ValueType, class Func>
Node<KeyType, ValueType>& HashTable<KeyType, ValueType, Func>::operator[](uint64_t index) const {
    if (index >= this->allocated_) {
        throw std::out_of_range("out_of_range");
    }
    if (!this->data_[index]) {
        throw std::runtime_error("runtime_error");
    }
    return *(this->data_[index]);
}

template <class KeyType, class ValueType, class Func>
Node<KeyType, ValueType> HashTable<KeyType, ValueType, Func>::at(uint64_t index) const {
    return this->operator[](index);
}

template <class KeyType, class ValueType, class Func>
void HashTable<KeyType, ValueType, Func>::erase(KeyType key) {
    size_t index = this->hasher_(key) % this->allocated_;
    Node<KeyType, ValueType>* current = this->data_[index];
    if (!current) {
        return;
    }
    if (current->key == key) {
        this->data_[index] = current->next;
        current->next = nullptr;
        delete current;
        --this->length_;
        return;
    }
    while (current->next && current->next->key != key) {
        current = current->next;
    }
    if (!current->next) {
        return;
    }
    Node<KeyType, ValueType>* to_delete = current->next;
    current->next = to_delete->next;
    to_delete->next = nullptr;
    --this->length_;
    delete to_delete;
}