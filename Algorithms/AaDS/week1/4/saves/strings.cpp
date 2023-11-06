#include <iostream>
#include <vector>
#include <cstdint>

class BigInt {
public:
    BigInt() = default;
    BigInt(uint64_t number);
    BigInt(const std::string& number);

    inline unsigned char operator[](int digit_number) const;
    inline unsigned char lastDigit() const;
    inline int length() const;
    inline void setDigit(int digit_number, unsigned char value) const;
    const uint64_t toNumber() const;

    friend std::istream& operator>>(std::istream& outstream, BigInt& number);
    friend std::ostream& operator<<(std::ostream& outstream, const BigInt& number);

    BigInt& operator+=(const BigInt& right);
    friend BigInt operator+(BigInt left, const BigInt& right);

    BigInt& operator-=(const BigInt& right);
    friend BigInt operator-(BigInt left, const BigInt& right);

    BigInt& operator*=(const BigInt& right);
    friend BigInt operator*(BigInt left, const BigInt& right);

private:
    mutable std::string value_;

    inline void normalize(const int desired_length) const;
};

BigInt::BigInt(uint64_t number) {
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

inline unsigned char BigInt::operator[](int digit_number) const {
    if (digit_number < 0 || digit_number >= this->length()) {
        return 0;
    }
    return this->value_[digit_number];
}
inline unsigned char BigInt::lastDigit() const {
    return (this->value_.back());
}
inline int BigInt::length() const {
    return this->value_.size();
}
inline void BigInt::setDigit(int digit_number, unsigned char value) const {
    if (digit_number < 0) {
        return;
    }
    if (digit_number >= this->length()) {
        this->normalize(digit_number + 1);
    }
    this->value_[digit_number] = value;
}
const uint64_t BigInt::toNumber() const {
    uint64_t res = 0, power = 1;
    for (int i = 0; i < this->length(); i++) {
        res += (*this)[i] * power;
        power *= 10;
    }
    return res;
}
inline void BigInt::normalize(int desired_length) const {
    if (desired_length > this->length()) {
        this->value_.append(desired_length - this->length(), static_cast<char>(0));
    } else {
        int last_not_0 = this->value_.find_last_not_of(static_cast<char>(0));
        this->value_.erase(this->value_.begin() + std::max(desired_length, last_not_0 + 1),
                           this->value_.end());
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
    unsigned char extra = 0;
    for (int i = 0; i < this->length(); i++) {
        unsigned char sum = (*this)[i] + right[i] + extra;
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
    char taken = 0;
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
    int part_size = (std::max(this->length(), right.length()) + 1) >> 1;
    if (this->length() + right.length() < 19) {
        uint64_t result = this->toNumber() * right.toNumber();
        int i = 0;
        this->value_.clear();
        while (result > 0) {
            this->value_.push_back(result % 10);
            result /= 10;
        }
        return *this;
    }
    if (this->length() == 1 && right.length() == 1) {
        unsigned char result = (*this)[0] * right[0];
        this->setDigit(0, result % 10);
        this->setDigit(1, result / 10);
        return *this;
    }
    BigInt a, b, c, d;
    a.value_.reserve(part_size);
    b.value_.reserve(part_size);
    c.value_.reserve(part_size);
    d.value_.reserve(part_size);
    for (int i = 0; i < part_size; i++) {
        b.value_.push_back((*this)[i]);
    }
    for (int i = part_size; i < (part_size << 1); i++) {
        a.value_.push_back((*this)[i]);
    }
    for (int i = 0; i < part_size; i++) {
        d.value_.push_back(right[i]);
    }
    for (int i = part_size; i < (part_size << 1); i++) {
        c.value_.push_back(right[i]);
    }

    BigInt ac = a, bd = b;
    ac *= c;
    bd *= d;
    a += b;
    c += d;
    a *= c;
    a -= ac;
    a -= bd;

    ac.normalize(part_size << 1);
    bd.normalize(part_size << 1);
    this->value_ = bd.value_ + ac.value_;

    unsigned char extra = 0;
    for (int i = part_size; i < this->length(); i++) {
        unsigned char sum = (*this)[i] + a[i - part_size] + extra;
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

#include <fstream>
#include <chrono>
int main(void) {

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::ifstream fin("input.txt");
    std::chrono::high_resolution_clock::time_point now1 = std::chrono::high_resolution_clock::now();

    BigInt a, b;
    fin >> a >> b;
    a *= b;
    std::cout << a;

    std::chrono::high_resolution_clock::time_point now2 = std::chrono::high_resolution_clock::now();
    std::cout << std::endl;
    std::cout << std::endl << double((now2 - now1).count()) / 1000000 << "ms";
    return 0;
}