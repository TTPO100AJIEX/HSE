#include <climits>
#include <string_view>
#include <set>
#include <vector>
#include <algorithm>
#include <queue>

class Trie
{
public:
    struct Alphabet
    {
        char minSymbol;
        char maxSymbol;
    };

    static std::vector< std::set<unsigned int> > findOccurences(std::string_view text, const std::vector<std::string_view>& patterns, Alphabet alphabet)
    {
        Trie trie(alphabet);
        for (unsigned int i = 0; i < patterns.size(); ++i) trie.add(patterns[i], i);
        trie.calculateSuffixLinks();

        std::vector< std::set<unsigned int> > answer(patterns.size());
        const Trie* current = &trie;
        for (unsigned int i = 0; i < text.size(); ++i)
        {
            current = current->next(text[i] - alphabet.minSymbol);

            const Trie* cur = current;
            if (cur->isTerminal)
            {
                for (unsigned int index : cur->patternIndexes) answer[index].insert(i + 1 - patterns[index].size());
            }
            while (cur != cur->suffixLink && cur->isLinkTerminal)
            {
                cur = cur->suffixLink;
                if (cur->isTerminal)
                {
                    for (unsigned int index : cur->patternIndexes) answer[index].insert(i + 1 - patterns[index].size());
                }
            }
        }
        return answer;
    }

private:
    Trie* parent = nullptr;
    std::vector<Trie*> children;
    Trie* suffixLink = nullptr;
    int symbol = 0;
    Alphabet alphabet;
    bool isTerminal = false;
    bool isLinkTerminal = false;
    std::vector<unsigned int> patternIndexes;
    
    Trie(Alphabet alphabet, Trie* parent = nullptr, int symbol = 0)
        : parent(parent), symbol(symbol), alphabet(alphabet)
    {
        this->children.resize(alphabet.maxSymbol - alphabet.minSymbol + 1);
    };

    void add(std::string_view pattern, unsigned int index)
    {
        if (pattern.size() == 0) { this->isTerminal = true; this->patternIndexes.push_back(index); return; }
        int symbol = pattern[0] - this->alphabet.minSymbol;
        if (!this->children[symbol]) this->children[symbol] = new Trie(this->alphabet, this, symbol);
        this->children[symbol]->add(pattern.substr(1), index);
    }
    

    Trie* next(int symbol) const
    {
        if (symbol >= 0 && (unsigned int)(symbol) < this->children.size() && this->children[symbol]) return this->children[symbol];
        if (this == this->suffixLink) return this->suffixLink;
        return this->suffixLink->next(symbol);
    }

    void calculateSuffixLink()
    {
        if (!this->parent) // root
        {
            this->suffixLink = this;
            this->isLinkTerminal = this->suffixLink->isTerminal;
            return;
        }
        this->suffixLink = this->parent->suffixLink->next(this->symbol);
        if (this->suffixLink == this) this->suffixLink = this->parent; // first level
        this->isLinkTerminal = this->suffixLink->isTerminal || this->suffixLink->isLinkTerminal;
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
    
    ~Trie()
    {
        for (Trie* child : this->children) delete child;
    }
};

std::set<unsigned int> search(std::string_view text, std::string_view pattern)
{
    std::set<unsigned int> answer;
    if (text.size() < pattern.size()) return answer;
    
    // Split pattern into parts by substitution symbol
    std::vector<std::string_view> parts;
    unsigned int partStart = 0;
    Trie::Alphabet alphabet{ CHAR_MAX, 0 };
    for (unsigned int i = 0; i < pattern.size(); ++i)
    {
        if (pattern[i] == '?')
        {
            parts.push_back(pattern.substr(partStart, i - partStart));
            partStart = i + 1;
        }
        else
        {
            alphabet.minSymbol = std::min(alphabet.minSymbol, pattern[i]);
            alphabet.maxSymbol = std::max(alphabet.maxSymbol, pattern[i]);
        }
    }
    parts.push_back(pattern.substr(partStart, pattern.size() - partStart));
    
    std::vector< std::set<unsigned int> > occurences = Trie::findOccurences(text, parts, alphabet);
    for (unsigned int i = occurences.size() - 1; i > 0; --i)
    {
        std::set<unsigned int>::iterator iter = occurences[i - 1].begin();
        while (iter != occurences[i - 1].end())
        {
            if (!occurences[i].contains(*iter + parts[i - 1].size() + 1)) occurences[i - 1].erase(iter++);
            else iter++;
        }
    }
    return occurences[0];
}