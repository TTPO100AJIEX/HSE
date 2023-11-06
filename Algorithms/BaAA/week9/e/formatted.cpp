#include <ios>
#include <iostream>
#include <queue>

class Trie {
private:
    Trie* parent_ = nullptr;
    Trie* children_[2] = {nullptr};
    Trie* suffix_link_ = nullptr;
    bool symbol_;
    bool is_terminal_ = false;
    bool is_link_terminal_ = false;
    enum class Status { NONE, ENTERED, VISITED };
    Status status_ = Status::NONE;

    Trie* next(bool symbol) const {
        if (this->children_[symbol]) {
            return this->children_[symbol];
        }
        if (this == this->suffix_link_) {
            return this->suffix_link_;
        }
        return this->suffix_link_->next(symbol);
    }

    void calculateSuffixLink() {
        if (!this->parent_)  // root
        {
            this->suffix_link_ = this;
            return;
        }
        this->suffix_link_ = this->parent_->suffix_link_->next(this->symbol_);
        if (this->suffix_link_ == this)  // first level
        {
            this->suffix_link_ = this->parent_;
            return;
        }
        this->is_link_terminal_ =
            this->suffix_link_->is_terminal_ || this->suffix_link_->is_link_terminal_;
    }

public:
    Trie(Trie* parent = nullptr, bool symbol = 0) : parent_(parent), symbol_(symbol){};

    void add() {
        char symbol;
        std::cin >> symbol;
        if (symbol != '0' && symbol != '1') {
            this->is_terminal_ = true;
            return;
        }
        bool index = symbol - '0';
        if (!this->children_[index]) {
            this->children_[index] = new Trie(this, index);
        }
        this->children_[index]->add();
    }

    void calculateSuffixLinks() {
        this->calculateSuffixLink();
        std::queue<Trie*> updated;
        updated.push(this);
        while (!updated.empty()) {
            Trie* current = updated.front();
            updated.pop();
            for (Trie* child : current->children_) {
                if (!child) {
                    continue;
                }
                child->calculateSuffixLink();
                updated.push(child);
            }
        }
    }

    bool hasLoops() {
        if (this->is_link_terminal_ || this->is_terminal_ || this->status_ == Status::VISITED) {
            return false;
        }
        if (this->status_ == Status::ENTERED) {
            return true;
        }
        this->status_ = Status::ENTERED;
        bool result = this->next(0)->hasLoops() || this->next(1)->hasLoops();
        this->status_ = Status::VISITED;
        return result;
    }

    ~Trie() {
        delete this->children_[0];
        delete this->children_[1];
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

    if (trie.hasLoops()) {
        std::cout << "TAK";
    } else {
        std::cout << "NIE";
    }
}