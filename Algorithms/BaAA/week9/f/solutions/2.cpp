#include <ios>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <map>

constexpr unsigned int symbolsSize = 'z' - 'a' + 1;

class Trie
{
private:
    char symbol;
    unsigned int patternSize;
    Trie* parent = nullptr;
    std::map<char, Trie*> children;
    Trie* suffixLink = nullptr;
    bool isTerminal = false;
    bool isLinkTerminal = false;

    Trie* getChild(char symbol) const
    {
        std::map<char, Trie*>::const_iterator iter = this->children.find(symbol);
        if (iter == this->children.end()) return nullptr;
        return iter->second;
    }

    void calculateSuffixLink()
    {
        if (!this->parent) // root
        {
            this->suffixLink = this;
            return;
        }
        Trie* suffixLinkParent = this->parent->suffixLink;
        while (!suffixLinkParent->getChild(this->symbol) && suffixLinkParent != suffixLinkParent->suffixLink)
        {
            suffixLinkParent = suffixLinkParent->suffixLink;
        }
        Trie* suffixLink = suffixLinkParent->getChild(this->symbol);
        if (!suffixLink) // link to root
        {
            this->suffixLink = suffixLinkParent;
            return;
        }
        if (suffixLink == this) // first level
        {
            this->suffixLink = this->parent;
            return;
        }
        this->suffixLink = suffixLink;
        this->isLinkTerminal = suffixLink->isTerminal || suffixLink->isLinkTerminal;
    }


public:
    Trie(Trie* parent = nullptr, char symbol = 0, unsigned int patternSize = 0) : symbol(symbol), patternSize(patternSize), parent(parent) { };

    void add()
    {
        char symbol; std::cin >> symbol;
        if (symbol < 'a' || symbol > 'z') { this->isTerminal = true; return; }
        Trie* child = this->getChild(symbol);
        if (!child) child = this->children.insert_or_assign(symbol, new Trie(this, symbol, this->patternSize + 1)).first->second;
        child->add();
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
            for (const std::pair<const char, Trie*>& child : current->children)
            {
                child.second->calculateSuffixLink();
                updated.push(child.second);
            }
        }
    }


    const Trie* accept(char symbol) const
    {
        Trie* child = this->getChild(symbol);
        if (child) return child;
        if (this == this->suffixLink) return this;
        return this->suffixLink->accept(symbol);
    }
    std::vector <unsigned int> getTerminalSizes() const
    {
        std::vector <unsigned int> answer;
        const Trie* current = this;
        if (current->isTerminal) answer.push_back(current->patternSize);
        while (current->isLinkTerminal)
        {
            current = current->suffixLink;
            if (current->isTerminal) answer.push_back(current->patternSize);
        }
        return answer;
    }


    ~Trie()
    {
        for (const std::pair<const char, Trie*>& child : this->children) delete child.second;
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

    std::vector<unsigned int> substrings;
    const Trie* current = &trie;
    for (char symbol = std::cin.get(); symbol >= 'a' && symbol <= 'z'; symbol = std::cin.get())
    {
        substrings.push_back(0);
        current = current->accept(symbol);
        std::vector<unsigned int> sizes = current->getTerminalSizes();
        for (unsigned int size : sizes) substrings[substrings.size() - size] = size;
    }
    std::cin >> std::skipws;

    unsigned int m; std::cin >> m;
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int l, r; std::cin >> l >> r;
        --l;
        int answer = 0;
        while(l < r)
        {
            if (substrings[l] == 0) break;
            l += substrings[l];
            ++answer;
        }
        if (l != r) answer = -1;
        std::cout << answer << '\n';
    }

    unsigned int q; std::cin >> q;
    if (q != 0) std::cout << 3;
}