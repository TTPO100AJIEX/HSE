#include <cstdlib>
#include <cstring>
#include <climits>
#include <utility>
#include <initializer_list>
#include <iterator>
#include <stdexcept>

class Vector {
    size_t size_ = 0, capacity_ = 0;
    int* arr_ = nullptr;

    void allocate(size_t desired);
    void reallocate();

public:
    // Стандартный конструктор.
    Vector();
    // Конструктор с указанием размера.
    explicit Vector(size_t n_size);
    // Конструктор на массиве.
    Vector(const int* vals, size_t size);
    // Конструктор на векторе.
    Vector(const Vector& vec);
    // Конструктор на initializer list'e.
    Vector(std::initializer_list<int> vals);

    // Деструктор.
    ~Vector();

    // Структура Итератора.
    struct Iterator {
        using IteratorCategory = std::random_access_iterator_tag;
        using DifferenceType = std::ptrdiff_t;

        // Конструктор итератора.
        explicit Iterator(int* ptr);

        // Операторы доступа.
        int& operator*() const;
        int* operator->();

        // Префиксный инкремент.
        Iterator& operator++();
        // Постфиксный инкремент.
        Iterator operator++(int);
        // Префиксный декремент.
        Iterator& operator--();
        // Постфиксный декремент.
        Iterator operator--(int);
        // Оператор произвольного шага вперёд.
        Iterator operator+(const DifferenceType& movement);
        // Оператор произвольного шага назад.
        Iterator operator-(const DifferenceType& movement);
        // Произвольный шаг вперёд с переприсваиванием.
        Iterator& operator+=(const DifferenceType& movement);
        // Произвольный шаг назад с переприсваиванием.
        Iterator& operator-=(const DifferenceType& movement);

        // Оператор сравнения.
        friend bool operator==(const Iterator& a, const Iterator& b);
        // Оператор неравенства.
        friend bool operator!=(const Iterator& a, const Iterator& b);

        friend const DifferenceType operator-(const Iterator& left, const Iterator& right);

    private:
        int* m_ptr_;
    };

    // Метод получения размера.
    size_t getSize() const;
    // Метод получения capacity__.
    size_t getCapacity() const;
    // Метод проверки на пустоту.
    bool isEmpty() const;

    // Метод изменения размера.
    void resize(size_t n_size);
    // Метод добавления в конец.
    void pushBack(int value);
    // Метод убирания из конца массива.
    void popBack();
    // Метод чистки массива.
    void clear();
    // Метод вставки в массив.
    void insert(size_t pos, int value);
    // Метод удаления с произвольной позиции.
    void erase(size_t pos);

    // Метод получения элемента по позиции.
    int at(size_t pos);
    // Метод получения первого элемента.
    int front();
    // Метод получения последнего элемента.
    int back();
    // Оператор доступа по индексу.
    int& operator[](size_t pos) const;
    // Метод, предоставляющий итератор начала.
    Iterator begin();
    // Метод, предоставляющий итератор конца массива.
    Iterator end();

    // Оператор присваивания.
    Vector& operator=(const Vector& other);
};

void Vector::allocate(size_t desired) {
    if (desired <= this->capacity_) {
        return;
    }
    this->arr_ = static_cast<int*>(std::realloc(this->arr_, desired * 2 * sizeof(int)));
    this->capacity_ = desired * 2;
}
void Vector::reallocate() {
    this->capacity_ <<= 1;
    this->arr_ = static_cast<int*>(std::realloc(this->arr_, this->capacity_ * sizeof(int)));
}

Vector::Vector() {
    this->allocate(10);
    std::memset(this->arr_, 0, 10 * sizeof(int));
}
Vector::Vector(size_t n_size) {
    this->allocate(n_size);
    this->size_ = n_size;
    std::memset(this->arr_, 0, n_size * sizeof(int));
}
Vector::Vector(const int* vals, size_t size) {
    this->allocate(size);
    this->size_ = size;
    std::memcpy(this->arr_, vals, size * sizeof(int));
}
Vector::Vector(const Vector& vec) : Vector(vec.arr_, vec.size_) {
}
Vector::Vector(std::initializer_list<int> vals) : Vector(std::data(vals), vals.size()) {
}

Vector::~Vector() {
    free(this->arr_);
}

Vector::Iterator::Iterator(int* ptr) : m_ptr_(ptr){};

int& Vector::Iterator::operator*() const {
    return *(this->m_ptr_);
}
int* Vector::Iterator::operator->() {
    return this->m_ptr_;
}

Vector::Iterator& Vector::Iterator::operator++() {
    this->m_ptr_++;
    return *this;
}
Vector::Iterator Vector::Iterator::operator++(int) {
    Vector::Iterator old = *this;
    this->m_ptr_++;
    return old;
}
Vector::Iterator& Vector::Iterator::operator--() {
    this->m_ptr_--;
    return *this;
}
Vector::Iterator Vector::Iterator::operator--(int) {
    Vector::Iterator old = *this;
    this->m_ptr_--;
    return old;
}

Vector::Iterator Vector::Iterator::operator+(const DifferenceType& movement) {
    Vector::Iterator new_iter = *this;
    new_iter += movement;
    return new_iter;
}
Vector::Iterator Vector::Iterator::operator-(const DifferenceType& movement) {
    Vector::Iterator new_iter = *this;
    new_iter -= movement;
    return new_iter;
}
Vector::Iterator& Vector::Iterator::operator+=(const DifferenceType& movement) {
    this->m_ptr_ += movement;
    return *this;
}
Vector::Iterator& Vector::Iterator::operator-=(const DifferenceType& movement) {
    this->m_ptr_ -= movement;
    return *this;
}

const Vector::Iterator::DifferenceType operator-(const Vector::Iterator& left,
                                                 const Vector::Iterator& right) {
    return (left.m_ptr_ - right.m_ptr_);
}

bool operator==(const Vector::Iterator& a, const Vector::Iterator& b) {
    return (a.m_ptr_ == b.m_ptr_);
}
bool operator!=(const Vector::Iterator& a, const Vector::Iterator& b) {
    return (a.m_ptr_ != b.m_ptr_);
}

size_t Vector::getSize() const {
    return this->size_;
}
size_t Vector::getCapacity() const {
    return this->capacity_;
}
bool Vector::isEmpty() const {
    return (this->size_ == 0);
}

void Vector::resize(size_t n_size) {
    this->allocate(n_size);
    if (n_size > this->size_) {
        std::memset(this->arr_ + this->size_, 0, (n_size - this->size_) * sizeof(int));
    }
    this->size_ = n_size;
}
void Vector::pushBack(int value) {
    if (this->size_ == this->capacity_) {
        this->reallocate();
    }
    this->arr_[this->size_++] = value;
}
void Vector::popBack() {
    if (this->size_ == 0) {
        throw std::runtime_error("Empty Array!");
    }
    this->size_--;
}
void Vector::clear() {
    this->size_ = 0;
}
void Vector::insert(size_t pos, int value) {
    if (pos > this->size_) {
        throw std::runtime_error("Wrong Position!");
    }
    if (this->size_ == this->capacity_) {
        this->reallocate();
    }
    this->size_++;
    for (; pos < this->size_; pos++) {
        std::swap(value, this->arr_[pos]);
    }
}
void Vector::erase(size_t pos) {
    if (this->size_ == 0) {
        throw std::runtime_error("Empty Array!");
    }
    if (pos >= this->size_) {
        throw std::runtime_error("Wrong Position!");
    }
    this->size_--;
    for (; pos < this->size_; pos++) {
        this->arr_[pos] = this->arr_[pos + 1];
    }
}

int Vector::at(size_t pos) {
    if (pos >= this->size_) {
        throw std::runtime_error("Wrong Position!");
    }
    return this->arr_[pos];
}
int Vector::front() {
    if (this->size_ == 0) {
        throw std::runtime_error("Empty Array!");
    }
    return this->arr_[0];
}
int Vector::back() {
    if (this->size_ == 0) {
        throw std::runtime_error("Empty Array!");
    }
    return this->arr_[this->size_ - 1];
}
int& Vector::operator[](size_t pos) const {
    if (pos >= this->size_) {
        throw std::runtime_error("Wrong Position!");
    }
    return this->arr_[pos];
}
Vector::Iterator Vector::begin() {
    return Vector::Iterator(this->arr_);
}
Vector::Iterator Vector::end() {
    return Vector::Iterator(this->arr_ + this->size_);
}

Vector& Vector::operator=(const Vector& other) {
    this->allocate(other.size_);
    this->size_ = other.size_;
    std::memcpy(this->arr_, other.arr_, this->size_ * sizeof(int));
    return *this;
}

// Сортировка слиянием.
void mergeSort(Vector::Iterator begin, const Vector::Iterator end) {
    if (end == begin + 1) {
        return;
    }
    size_t size = end - begin;
    Vector::Iterator middle = begin + size / 2;
    mergeSort(begin, middle);
    mergeSort(middle, end);
    Vector merged(size);
    Vector::Iterator left_part = begin, right_part = middle;
    for (Vector::Iterator iter = merged.begin(); iter != merged.end(); iter++) {
        if (right_part == end || (left_part != middle && *left_part < *right_part)) {
            *iter = *(left_part++);
        } else {
            *iter = *(right_part++);
        }
    }
    std::memcpy(begin.operator->(), merged.begin().operator->(), merged.getSize() * sizeof(int));
}
// Сортировка вставками.
void insertionSort(Vector::Iterator begin, Vector::Iterator end) {
    for (Vector::Iterator iter = begin + 1; iter != end; iter++) {
        Vector::Iterator left = begin, right = iter;
        int element = *iter;
        while (left + 1 != right) {
            Vector::Iterator middle = left + (right - left) / 2;
            if (*middle > element) {
                right = middle;
            } else {
                left = middle;
            }
        }
        if (*left <= element) {
            left++;
        }
        while (left != iter) {
            std::swap(*(left++), *iter);
        }
    }
}