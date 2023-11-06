#include <ios>
#include <iostream>
#include <queue>

class Trie
{
private:
    Trie* parent = nullptr;
    Trie* children[2] = { nullptr };
    Trie* suffixLink = nullptr;
    bool symbol;
    bool isTerminal = false;
    bool isLinkTerminal = false;
    enum class Status { NONE, ENTERED, VISITED };
    Status status = Status::NONE;
    
    Trie* next(bool symbol) const
    {
        if (this->children[symbol]) return this->children[symbol];
        if (this == this->suffixLink) return this->suffixLink;
        return this->suffixLink->next(symbol);
    }

    void calculateSuffixLink()
    {
        if (!this->parent) // root
        {
            this->suffixLink = this;
            return;
        }
        this->suffixLink = this->parent->suffixLink->next(this->symbol);
        if (this->suffixLink == this) // first level
        {
            this->suffixLink = this->parent;
            return;
        }
        this->isLinkTerminal = this->suffixLink->isTerminal || this->suffixLink->isLinkTerminal;
    }


public:
    Trie(Trie* parent = nullptr, bool symbol = 0) : parent(parent), symbol(symbol) { };

    void add()
    {
        char symbol; std::cin >> symbol;
        if (symbol != '0' && symbol != '1') { this->isTerminal = true; return; }
        bool index = symbol - '0';
        if (!this->children[index]) this->children[index] = new Trie(this, index);
        this->children[index]->add();
    }

    void calculateSuffixLinks()
    {
        this->calculateSuffixLink();
        std::queue<Trie*> updated;
        updated.push(this);
        while (!updated.empty())
        {
            Trie* current = updated.front();
            updated.pop();
            for (Trie* child : current->children)
            {
                if (!child) continue;
                child->calculateSuffixLink();
                updated.push(child);
            }
        }
    }

    bool hasLoops()
    {
        if (this->isLinkTerminal || this->isTerminal || this->status == Status::VISITED) return false;
        if (this->status == Status::ENTERED) return true;
        this->status = Status::ENTERED;
        bool result = this->next(0)->hasLoops() || this->next(1)->hasLoops();
        this->status = Status::VISITED;
        return result;
    }

    ~Trie()
    {
        delete this->children[0];
        delete this->children[1];
    }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n; std::cin >> n;
    std::cin.get();
    std::cin >> std::noskipws;
    Trie trie;
    for (unsigned int i = 0; i < n; ++i) trie.add();
    trie.calculateSuffixLinks();
    
    if (trie.hasLoops()) std::cout << "TAK";
    else std::cout << "NIE";
}