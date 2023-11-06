#include <iostream>
#include <vector>
#include <cstdint>

class BigInt {
public:
    BigInt() = default;
    BigInt(unsigned int number);
    BigInt(const std::string& number);

    inline uint8_t operator[](int digit_number) const;
    inline uint8_t lastDigit() const;
    inline int length() const;
    inline void setDigit(int digit_number, uint8_t value) const;

    friend std::istream& operator>>(std::istream& outstream, BigInt& number);
    friend std::ostream& operator<<(std::ostream& outstream, const BigInt& number);

    BigInt& operator+=(const BigInt& right);
    friend BigInt operator+(BigInt left, const BigInt& right);

    BigInt& operator-=(const BigInt& right);
    friend BigInt operator-(BigInt left, const BigInt& right);

    BigInt& operator*=(const BigInt& right);
    friend BigInt operator*(BigInt left, const BigInt& right);

private:
    mutable std::vector<uint8_t> value_;

    inline void normalize(int desired_length) const;
};

BigInt::BigInt(unsigned int number) {
    while (number > 0) {
        this->value_.push_back(number % 10);
        number /= 10;
    }
}
BigInt::BigInt(const std::string& value) {
    for (std::string::const_reverse_iterator iter = value.rbegin(); iter != value.rend(); iter++) {
        this->value_.push_back(*iter - '0');
    }
}

inline uint8_t BigInt::operator[](int digit_number) const {
    if (digit_number < 0 || digit_number >= this->length()) {
        return 0;
    }
    return this->value_[digit_number];
}
inline uint8_t BigInt::lastDigit() const {
    return (this->value_.back());
}
inline int BigInt::length() const {
    return this->value_.size();
}
inline void BigInt::setDigit(int digit_number, uint8_t value) const {
    if (digit_number < 0) {
        return;
    }
    if (digit_number >= this->length()) {
        this->normalize(digit_number + 1);
    }
    this->value_[digit_number] = value;
}
inline void BigInt::normalize(int desired_length) const {
    if (desired_length > this->length()) {
        this->value_.reserve(desired_length);
        while (desired_length > this->length()) {
            this->value_.push_back(0);
        }
    } else {
        while (desired_length < this->length() && this->lastDigit() == 0) {
            this->value_.pop_back();
        }
    }
}

std::istream& operator>>(std::istream& instream, BigInt& number) {
    std::string value;
    instream >> value;
    number = BigInt(value);
    return instream;
}
std::ostream& operator<<(std::ostream& outstream, const BigInt& number) {
    number.normalize(1);
    for (int i = number.length() - 1; i >= 0; i--) {
        outstream << static_cast<unsigned int>(number[i]);
    }
    return outstream;
}

BigInt& BigInt::operator+=(const BigInt& right) {
    this->normalize(std::max(this->length(), right.length()) + 1);
    uint8_t extra = 0;
    for (int i = 0; i < this->length(); i++) {
        uint8_t sum = (*this)[i] + right[i] + extra;
        extra = 0;
        if (sum >= 10) {
            sum -= 10;
            extra = 1;
        }
        this->setDigit(i, sum);
    }
    return *this;
}
BigInt operator+(BigInt left, const BigInt& right) {
    return left += right;
}

BigInt& BigInt::operator-=(const BigInt& right) {
    int8_t taken = 0;
    for (int i = 0; i < this->length(); i++) {
        if ((*this)[i] < right[i] + taken) {
            this->setDigit(i, 10 + (*this)[i] - right[i] - taken);
            taken = 1;
        } else {
            this->setDigit(i, (*this)[i] - right[i] - taken);
            taken = 0;
        }
    }
    return *this;
}
BigInt operator-(BigInt left, const BigInt& right) {
    return left -= right;
}

BigInt& BigInt::operator*=(const BigInt& right) {
    this->normalize(1);
    right.normalize(1);
    if (this->length() == 1 && right.length() == 1) {
        uint8_t result = (*this)[0] * right[0];
        this->setDigit(0, result % 10);
        this->setDigit(1, result / 10);
        return *this;
    }
    int part_size = (std::max(this->length(), right.length()) + 1) >> 1;
    BigInt a, b, c, d;
    a.value_.reserve(part_size);
    b.value_.reserve(part_size);
    c.value_.reserve(part_size);
    d.value_.reserve(part_size);
    for (int i = 0; i < part_size; i++) {
        b.value_.push_back((*this)[i]);
    }
    for (int i = part_size; i < 2 * part_size; i++) {
        a.value_.push_back((*this)[i]);
    }
    for (int i = 0; i < part_size; i++) {
        d.value_.push_back(right[i]);
    }
    for (int i = part_size; i < 2 * part_size; i++) {
        c.value_.push_back(right[i]);
    }

    BigInt ac = a * c, bd = b * d;
    a += b;
    c += d;
    a *= c;
    a -= ac;
    a -= bd;

    ac.normalize(2 * part_size);
    bd.normalize(2 * part_size);
    this->value_.resize(0);
    this->value_.reserve(4 * part_size);
    for (int i = 0; i < bd.length(); i++) {
        this->value_.push_back(bd[i]);
    }
    for (int i = 0; i < ac.length(); i++) {
        this->value_.push_back(ac[i]);
    }

    uint8_t extra = 0;
    for (int i = part_size; i < this->length(); i++) {
        uint8_t sum = (*this)[i] + a[i - part_size] + extra;
        extra = 0;
        if (sum >= 10) {
            sum -= 10;
            extra = 1;
        }
        this->setDigit(i, sum);
    }

    return *this;
}
BigInt operator*(BigInt left, const BigInt& right) {
    return left *= right;
}

int main(void) {

    BigInt a, b;
    std::cin >> a >> b;
    a *= b;
    std::cout << a;

    return 0;
}