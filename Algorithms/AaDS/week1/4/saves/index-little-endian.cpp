#include <iostream>
#include <vector>
#include <cstdint>

class BigInt
{
public:
    BigInt() = default;

    inline size_t length() const;
    
    friend std::istream& operator>> (std::istream& outstream, BigInt& number);
    friend std::ostream& operator<< (std::ostream& outstream, const BigInt& number);
    
    BigInt& operator+= (const BigInt& right);
    friend BigInt operator+ (BigInt left, const BigInt& right);

    BigInt& operator-= (const BigInt& right);
    friend BigInt operator- (BigInt left, const BigInt& right);

    BigInt& operator*= (const BigInt& right);
    friend BigInt operator* (BigInt left, const BigInt& right);

private:
    mutable std::vector <uint8_t> value_;

    static inline std::pair <uint8_t, uint8_t> split_pair(uint8_t pair);
    static inline std::pair <int8_t, int8_t> split_pair_signed(uint8_t pair);
    static inline uint8_t merge_pair(uint8_t low, uint8_t high);
    
    inline void normalize(size_t desired_length) const;
};

inline std::pair <uint8_t, uint8_t> BigInt::split_pair(uint8_t pair)
{
    return std::make_pair(pair >> 4, pair & 0xf);
}
inline std::pair <int8_t, int8_t> BigInt::split_pair_signed(uint8_t pair)
{
    return std::make_pair(pair >> 4, pair & 0xf);
}
inline uint8_t BigInt::merge_pair(uint8_t low, uint8_t high)
{
    return(high + (low << 4));
}

inline size_t BigInt::length() const
{
    uint8_t last_digit = (this->value_.back() << 4);
    return(2 * this->value_.size() - (last_digit == 0));
}

std::istream& operator>>(std::istream& instream, BigInt& number)
{
    std::string value;
    instream >> value;
    number.value_.reserve((value.size() + 1) >> 1);
    for (std::string::reverse_iterator read_it = value.rbegin(); read_it != value.rend(); ++read_it)
    {
        uint8_t low = *read_it - '0', high = read_it + 1 == value.rend() ? 0 : *(++read_it) - '0';
        number.value_.push_back(BigInt::merge_pair(low, high));
    }
    return instream;
}
std::ostream& operator<<(std::ostream& outstream, const BigInt& number)
{
    bool skip_zeros = true;
    for (std::vector<uint8_t>::const_reverse_iterator iter = number.value_.rbegin(); iter != number.value_.rend(); ++iter)
    {
        auto [ low, high ] = BigInt::split_pair(*iter);
        if (!skip_zeros) outstream << (unsigned int)(high) << (unsigned int)(low);
        else
        {
            if (high == 0 && low == 0) continue;
            if (high == 0) outstream << (unsigned int)(low);
            else outstream << (unsigned int)(high) << (unsigned int)(low);
            skip_zeros = false;
        }
    }
    return outstream;
}

inline void BigInt::normalize(size_t desired_length) const
{
    desired_length = (desired_length + 1) >> 1;
    if (desired_length <= this->value_.size())
    {
        while (this->value_.back() == 0 && desired_length < this->value_.size()) this->value_.pop_back();
        return;
    }
    this->value_.resize(desired_length);
}

BigInt& BigInt::operator+= (const BigInt& right)
{
    this->normalize(std::max(right.length(), this->length()) + 1);
    uint8_t extra = 0;
    std::vector<uint8_t>::iterator this_it = this->value_.begin();
    for (std::vector<uint8_t>::const_iterator right_it = right.value_.begin(); right_it != right.value_.end(); ++right_it, ++this_it)
    {
        auto [ this_low, this_high ] = BigInt::split_pair(*this_it);
        auto [ right_low, right_high ] = BigInt::split_pair(*right_it);
        this_low += right_low + extra;
        extra = 0;
        if (this_low > 9) { this_low = (this_low + 6) & 0xf; extra = 1; }
        this_high += right_high + extra;
        extra = 0;
        if (this_high > 9) { this_high = (this_high + 6) & 0xf; extra = 1; }
        *this_it = BigInt::merge_pair(this_low, this_high);
    }
    for ( ; extra != 0; ++this_it)
    {
        auto [ this_low, this_high ] = BigInt::split_pair(*this_it);
        this_low += extra;
        extra = 0;
        if (this_low > 9) { this_low = (this_low + 6) & 0xf; extra = 1; }
        this_high += extra;
        extra = 0;
        if (this_high > 9) { this_high = (this_high + 6) & 0xf; extra = 1; }
        *this_it = BigInt::merge_pair(this_low, this_high);
    }
    this->normalize(1);
    return *this;
}
BigInt operator+ (BigInt left, const BigInt& right) { return left += right; }

BigInt& BigInt::operator-= (const BigInt& right)
{
    std::vector<uint8_t>::iterator this_it = this->value_.begin();
    for (std::vector<uint8_t>::const_iterator right_it = right.value_.begin(); right_it != right.value_.end(); ++right_it, ++this_it)
    {
        auto [ this_low, this_high ] = BigInt::split_pair_signed(*this_it);
        auto [ right_low, right_high ] = BigInt::split_pair_signed(*right_it);
        if (this_low == 15) this_low = -1;
        this_low -= right_low;
        if (this_low < 0)
        {
            this_low += 10;
            this_high--;
        }
        this_high -= right_high;
        if (this_high < 0)
        {
            this_high += 10;
            auto [ this_next_low, this_next_high ] = BigInt::split_pair_signed(*(this_it + 1));
            this_next_low--;
            if (this_next_low < 0) this_next_low = 15;
            *(this_it + 1) = BigInt::merge_pair(this_next_low, this_next_high);
        }
        *this_it = BigInt::merge_pair(this_low, this_high);
    }
    for (; this_it != this->value_.end(); this_it++)
    {
        auto [ this_low, this_high ] = BigInt::split_pair_signed(*this_it);
        if (this_low == 15) this_low = -1;
        if (this_low < 0) { this_low += 10; this_high--; }
        if (this_high < 0)
        {
            this_high += 10;
            auto [ this_next_low, this_next_high ] = BigInt::split_pair_signed(*(this_it + 1));
            this_next_low--;
            if (this_next_low < 0) this_next_low = 15;
            *(this_it + 1) = BigInt::merge_pair(this_next_low, this_next_high);
            *this_it = BigInt::merge_pair(this_low, this_high);
            continue;
        }
        *this_it = BigInt::merge_pair(this_low, this_high);
        break;
    }
    this->normalize(1);
    return *this;
}
BigInt operator- (BigInt left, const BigInt& right) { return left -= right; }

BigInt& BigInt::operator*= (const BigInt& right)
{
    if (this->length() == 1 && right.length() == 1)
    {
        auto [ this_low, this_high ] = BigInt::split_pair_signed(this->value_.front());
        auto [ right_low, right_high ] = BigInt::split_pair_signed(right.value_.front());
        uint8_t result = this_low * right_low;
        uint8_t result_low = result % 10, result_high = result / 10;
        this->value_.resize(1);
        this->value_[0] = BigInt::merge_pair(result_low, result_high);
        return *this;
    }
    size_t part_size = std::max(this->value_.size(), right.value_.size());
    this->normalize(part_size << 1);
    right.normalize(part_size << 1);
    BigInt a, b, c, d;
    if (part_size % 2 == 0)
    {
        size_t value_size = part_size >> 1;
        std::vector <uint8_t>::iterator this_it = this->value_.begin();
        while (b.value_.size() < value_size) { b.value_.push_back(*this_it); this_it++; }
        while (a.value_.size() < value_size) { a.value_.push_back(*this_it); this_it++; }
        std::vector <uint8_t>::iterator right_it = right.value_.begin();
        while (d.value_.size() < value_size) { d.value_.push_back(*right_it); right_it++; }
        while (c.value_.size() < value_size) { c.value_.push_back(*right_it); right_it++; }
    }
    else
    {
        size_t value_size = (part_size + 1) >> 1;
        std::vector <uint8_t>::iterator this_it = this->value_.begin();
        while (b.value_.size() != value_size - 1)
        {
            b.value_.push_back(*this_it);
            this_it++;
        }
        auto [ this_low, this_high ] = BigInt::split_pair_signed(*this_it);
        this_it++;
        b.value_.push_back(BigInt::merge_pair(this_low, 0));
        while (a.value_.size() != value_size - 1)
        {
            auto [ low, high ] = BigInt::split_pair_signed(*this_it);
            a.value_.push_back(BigInt::merge_pair(this_high, low));
            this_high = high;
            this_it++;
        }
        a.value_.push_back(BigInt::merge_pair(this_high, 0));

        std::vector <uint8_t>::iterator right_it = right.value_.begin();
        while (d.value_.size() != value_size - 1)
        {
            d.value_.push_back(*right_it);
            right_it++;
        }
        auto [ right_low, right_high ] = BigInt::split_pair_signed(*right_it);
        right_it++;
        d.value_.push_back(BigInt::merge_pair(right_low, 0));
        while (c.value_.size() != value_size - 1)
        {
            auto [ low, high ] = BigInt::split_pair_signed(*right_it);
            c.value_.push_back(BigInt::merge_pair(right_high, low));
            right_high = high;
            right_it++;
        }
        c.value_.push_back(BigInt::merge_pair(right_high, 0));
    }
    BigInt ac = a * c, bd = b * d;
    a += b; c += d;
    a *= c;
    a -= ac; a -= bd;
    ac.normalize(2 * part_size);
    bd.normalize(2 * part_size);
    this->value_.resize(0); this->value_.reserve(4 * part_size);
    for (std::vector<uint8_t>::iterator iter = bd.value_.begin(); iter != bd.value_.end(); iter++) this->value_.push_back(*iter);
    for (std::vector<uint8_t>::iterator iter = ac.value_.begin(); iter != ac.value_.end(); iter++) this->value_.push_back(*iter);
    BigInt a_copy;
    a_copy.value_.reserve(a.value_.size() * 2 + (part_size + 1) / 2);
    while (part_size >= 2)
    {
        a_copy.value_.push_back(0);
        part_size -= 2;
    }
    if (part_size == 1)
    {
        uint8_t save = 0;
        for (std::vector<uint8_t>::iterator iter = a.value_.begin(); iter != a.value_.end(); iter++)
        {
            auto [ low, high ] = BigInt::split_pair(*iter);
            a_copy.value_.push_back(BigInt::merge_pair(save, low));
            save = high;
        }
        a_copy.value_.push_back(BigInt::merge_pair(save, 0));
        a_copy.normalize(1);
    }
    else
    {
        for (std::vector<uint8_t>::iterator iter = a.value_.begin(); iter != a.value_.end(); iter++) a_copy.value_.push_back(*iter);
    }
    *this += a_copy;
    return *this;
}
BigInt operator* (BigInt left, const BigInt& right) { return left *= right; }

int main(void)
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    BigInt number1, number2;
    std::cin >> number1 >> number2;
    number1 *= number2;
    std::cout << number1;
    return 0;
}