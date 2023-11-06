#include <string>
#include <vector>
#include <functional>
#include <cstdlib>

class BloomFilter {
private:
    size_t hashes_, bits_;
    std::vector<uint8_t> data_;
    decltype(std::hash<std::string>{}) hasher_;

    const size_t hash(const std::string& obj, const size_t index) const {
        return (index == 0 ? this->hasher_(obj) : this->hasher_(obj + std::to_string(index))) %
               this->bits_;
    }

    class PrefixTree {
    private:
        struct Node {
            bool isEnd = false;
            Node* children[256];

            Node() {
                for (size_t i = 0; i < 256; ++i) {
                    this->children[i] = nullptr;
                }
            }
            ~Node() {
                for (size_t i = 0; i < 256; ++i) {
                    delete this->children[i];
                }
            }

            void add(const std::string& obj, size_t index) {
                if (index == obj.size()) {
                    this->isEnd = true;
                    return;
                }
                uint8_t ind = static_cast<uint8_t>(obj[index]);
                if (!this->children[ind]) {
                    this->children[ind] = new Node();
                }
                this->children[ind]->add(obj, index + 1);
            }
            bool has(const std::string& obj, size_t index) const {
                if (index == obj.size()) {
                    return this->isEnd;
                }
                uint8_t ind = static_cast<uint8_t>(obj[index]);
                return this->children[ind] ? this->children[ind]->has(obj, index + 1) : false;
            }
        };
        Node root_;

    public:
        void add(const std::string& obj) {
            this->root_.add(obj, 0);
        }
        bool has(const std::string& obj) const {
            return this->root_.has(obj, 0);
        }
    };
    mutable size_t queries_ = 0;
    mutable size_t false_positives_ = 0;
    mutable PrefixTree storage_;

public:
    BloomFilter(size_t hashes, size_t bits) : hashes_(hashes), bits_(bits) {
        data_.resize(bits, false);
    }
    ~BloomFilter() = default;

    void add(const std::string& obj) {
        for (size_t i = 0; i < this->hashes_; ++i) {
            this->data_[this->hash(obj, i)] = true;
        }
        this->storage_.add(obj);
    }

    bool verify(const std::string& obj) const {
        this->queries_++;
        for (size_t i = 0; i < this->hashes_; ++i) {
            if (!this->data_[this->hash(obj, i)]) {
                return false;
            }
        }
        if (!this->storage_.has(obj)) {
            this->false_positives_++;
        }
        return true;
    }

    double getFPRate() const {
        return this->queries_ == 0 ? 0
                                   : static_cast<double>(this->false_positives_) / this->queries_;
    }
    size_t numberOfHashFunctions() const {
        return this->hashes_;
    }
    size_t numberOfBits() const {
        return this->bits_;
    }
};