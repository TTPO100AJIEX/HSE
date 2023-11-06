#include <string>
#include <vector>
#include <functional>
#include <cstdlib>

class BloomFilter
{
private:
    size_t hashes, bits;
    std::vector<bool> data;
    decltype(std::hash<std::string>{}) hasher;

    const size_t hash(const std::string& obj, const size_t index) const { return (index == 0 ? this->hasher(obj) : this->hasher(obj + std::to_string(index))) % this->bits; }

    class PrefixTree
    {
    private:
        struct Node
        {
            bool isEnd = false;
            Node* children[256];

            Node() { for (size_t i = 0; i < 256; ++i) this->children[i] = nullptr; }
            ~Node() { for (size_t i = 0; i < 256; ++i) delete this->children[i]; }
            
            void add(const std::string& obj, size_t index)
            {
                if (index == obj.size()) { this->isEnd = true; return; }
                uint8_t ind = (uint8_t)(obj[index]);
                if (!this->children[ind]) this->children[ind] = new Node();
                this->children[ind]->add(obj, index + 1);
            }
            bool has(const std::string& obj, size_t index) const
            {
                if (index == obj.size()) return this->isEnd;
                uint8_t ind = (uint8_t)(obj[index]);
                return this->children[ind] ? this->children[ind]->has(obj, index + 1) : false;
            }
        };
        Node root;
        
    public:
        void add(const std::string& obj) { this->root.add(obj, 0); }
        bool has(const std::string& obj) const { return this->root.has(obj, 0); }

    };
    mutable size_t queries = 0;
    mutable size_t false_positives = 0;
    mutable PrefixTree storage;

public:
    BloomFilter(size_t hashes, size_t bits) : hashes(hashes), bits(bits) { data.resize(bits, false); }
    ~BloomFilter() = default;

    void add(const std::string& obj)
    {
        for (size_t i = 0; i < this->hashes; ++i) this->data[this->hash(obj, i)] = true;
        this->storage.add(obj);
    }

    bool verify(const std::string& obj) const
    {
        this->queries++;
        for (size_t i = 0; i < this->hashes; ++i)
        {
            if (!this->data[this->hash(obj, i)]) return false;
        }
        if (!this->storage.has(obj)) this->false_positives++;
        return true;
    }

    double getFPRate() const { return this->queries == 0 ? 0 : (double)(this->false_positives) / this->queries; }
    size_t numberOfHashFunctions() const { return this->hashes; }
    size_t numberOfBits() const { return this->bits; }
};