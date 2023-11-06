#include <ios>
#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>

constexpr unsigned int kSymbolsSize = 'z' - 'a' + 1;

class Trie {
private:
    char symbol_;
    unsigned int pattern_size_;
    Trie* parent_ = nullptr;
    std::map<char, Trie*> children_;
    Trie* suffix_link_ = nullptr;
    bool is_terminal_ = false;
    bool is_link_terminal_ = false;

    Trie* getChild(char symbol) const {
        std::map<char, Trie*>::const_iterator iter = this->children_.find(symbol);
        if (iter == this->children_.end()) {
            return nullptr;
        }
        return iter->second;
    }

    void calculateSuffixLink() {
        if (!this->parent_)  // root
        {
            this->suffix_link_ = this;
            return;
        }
        Trie* suffix_link_parent = this->parent_->suffix_link_;
        while (!suffix_link_parent->getChild(this->symbol_) &&
               suffix_link_parent != suffix_link_parent->suffix_link_) {
            suffix_link_parent = suffix_link_parent->suffix_link_;
        }
        Trie* suffix_link = suffix_link_parent->getChild(this->symbol_);
        if (!suffix_link)  // link to root
        {
            this->suffix_link_ = suffix_link_parent;
            return;
        }
        if (suffix_link == this)  // first level
        {
            this->suffix_link_ = this->parent_;
            return;
        }
        this->suffix_link_ = suffix_link;
        this->is_link_terminal_ = suffix_link->is_terminal_ || suffix_link->is_link_terminal_;
    }

public:
    Trie(Trie* parent = nullptr, char symbol = 0, unsigned int pattern_size = 0)
        : symbol_(symbol), pattern_size_(pattern_size), parent_(parent){};

    void add() {
        char symbol;
        std::cin >> symbol;
        if (symbol < 'a' || symbol > 'z') {
            this->is_terminal_ = true;
            return;
        }
        Trie* child = this->getChild(symbol);
        if (!child) {
            child = this->children_
                        .insert_or_assign(symbol, new Trie(this, symbol, this->pattern_size_ + 1))
                        .first->second;
        }
        child->add();
    }

    void calculateSuffixLinks() {
        this->calculateSuffixLink();
        std::queue<Trie*> updated;
        updated.push(this);
        while (!updated.empty()) {
            Trie* current = updated.front();
            updated.pop();
            for (const std::pair<const char, Trie*>& child : current->children_) {
                child.second->calculateSuffixLink();
                updated.push(child.second);
            }
        }
    }

    const Trie* accept(char symbol) const {
        Trie* child = this->getChild(symbol);
        if (child) {
            return child;
        }
        if (this == this->suffix_link_) {
            return this;
        }
        return this->suffix_link_->accept(symbol);
    }
    std::vector<unsigned int> getTerminalSizes() const {
        std::vector<unsigned int> answer;
        const Trie* current = this;
        if (current->is_terminal_) {
            answer.push_back(current->pattern_size_);
        }
        while (current->is_link_terminal_) {
            current = current->suffix_link_;
            if (current->is_terminal_) {
                answer.push_back(current->pattern_size_);
            }
        }
        return answer;
    }

    ~Trie() {
        for (const std::pair<const char, Trie*>& child : this->children_) {
            delete child.second;
        }
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n;
    std::cin >> n;
    std::cin.get();
    std::cin >> std::noskipws;
    Trie trie;
    for (unsigned int i = 0; i < n; ++i) {
        trie.add();
    }
    trie.calculateSuffixLinks();

    std::vector<unsigned int> substrings;
    const Trie* current = &trie;
    for (char symbol = std::cin.get(); symbol >= 'a' && symbol <= 'z'; symbol = std::cin.get()) {
        substrings.push_back(0);
        current = current->accept(symbol);
        std::vector<unsigned int> sizes = current->getTerminalSizes();
        for (unsigned int size : sizes) {
            substrings[substrings.size() - size] = size;
        }
    }
    std::cin >> std::skipws;

    unsigned int m;
    std::cin >> m;
    for (unsigned int i = 0; i < m; ++i) {
        unsigned int l, r;
        std::cin >> l >> r;
        --l;
        int answer = 0;
        while (l < r) {
            if (substrings[l] == 0) {
                break;
            }
            l += substrings[l];
            ++answer;
        }
        if (l != r) {
            answer = -1;
        }
        std::cout << answer << '\n';
    }

    unsigned int k;
    std::cin >> k;
    for (unsigned int test = 0; test < k; ++test) {
        int q, a, b, c, d, e, l1, r1, ans = 0;
        std::cin >> q >> a >> b >> c >> d >> e >> l1 >> r1;
        for (int request = 0; request < q; ++request) {
            unsigned int l = std::min(l1 % substrings.size() + 1, r1 % substrings.size() + 1) - 1,
                         r = std::max(l1 % substrings.size() + 1, r1 % substrings.size() + 1);
            int answer = 0;
            while (l < r) {
                if (substrings[l] == 0) {
                    break;
                }
                l += substrings[l];
                ++answer;
            }
            if (l != r) {
                answer = -1;
            }

            if (answer >= 0) {
                ans = (ans + answer) % e;
            }
            l1 = (a * l1 + b) % e;
            r1 = (c * r1 + d + answer) % e;
        }
        std::cout << ans << '\n';
    }
}