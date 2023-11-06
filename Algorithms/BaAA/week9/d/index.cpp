#include <ios>
#include <ostream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <map>
#include <utility>

std::string ab_storage, ca_storage, bc_storage;
std::string_view ab;
std::string_view ca;
std::string_view bc;
std::vector< std::vector<unsigned int> > BCxAB, CAxBC;
std::vector<int> lastIndexes;
std::vector<int> lengths;

std::vector< std::vector<unsigned int> > buildLCSarray(std::string_view prefixes, std::string_view suffixes)
{
    std::vector< std::vector<unsigned int> > dpSufSuf(prefixes.size() + 1, std::vector<unsigned int>(suffixes.size() + 1, 0));
    for (int i = prefixes.size() - 1; i >= 0; --i)
    {
        for (int j = suffixes.size() - 1; j >= 0; --j)
        {
            if (prefixes[i] == suffixes[j]) dpSufSuf[i][j] = dpSufSuf[i + 1][j + 1] + 1;
        }
    }

    std::vector< std::vector<unsigned int> > dpPrefSuf(prefixes.size() + 1, std::vector<unsigned int>(suffixes.size() + 1, 0));
    for (unsigned int j = 0; j <= suffixes.size(); ++j)
    {
        unsigned int prefLen = 0;
        for (unsigned int i = 0; i <= prefixes.size(); ++i)
        {
            while (prefLen <= i && prefLen + dpSufSuf[prefLen][j] <= i) ++prefLen;
            dpPrefSuf[i][j] = i - prefLen + 1;
        }
    }
    
    std::vector< std::vector<unsigned int> > answer(prefixes.size() + 1, std::vector<unsigned int>(suffixes.size() + 1, 0));
    for (unsigned int i = 1; i <= prefixes.size(); ++i)
    {
        for (int j = suffixes.size() - 1; j >= 0; --j)
        {
            answer[i][j] = std::max(std::max(answer[i - 1][j], answer[i][j + 1]), dpPrefSuf[i - 1][j]);
        }
    }
    return answer;
}

class Trie
{
private:
    Trie* parent = nullptr;
    std::map<char, Trie*> children;
    bool status[2] = { false, false };
    unsigned int firstIndex = 3000;
    unsigned int lastIndex = 0;
    unsigned int length = 0;

    Trie* getChild(char symbol) const
    {
        std::map<char, Trie*>::const_iterator iter = this->children.find(symbol);
        if (iter == this->children.end()) return nullptr;
        return iter->second;
    }


public:
    Trie(Trie* parent = nullptr) : parent(parent) { };

    void add(std::string_view data, unsigned int status, unsigned int startIndex)
    {
        if (status == 0) this->firstIndex = std::min(this->firstIndex, startIndex);
        if (status == 1) this->lastIndex = std::max(this->lastIndex, startIndex);
        if (data.size() == 0)
        {
            this->status[status] = true;
            return;
        }
        Trie* child = this->getChild(data[0]);
        if (!child)
        {
            child = this->children.insert_or_assign(data[0], new Trie(this)).first->second;
            child->length = this->length + 1;
        }
        child->add(data.substr(1), status, startIndex);
    }

    std::pair<unsigned int, std::pair<bool, bool> > solve()
    {
        unsigned int best_answer = 0;
        for (std::pair<char, Trie*> child : this->children)
        {
            std::pair<unsigned int, std::pair<bool, bool> > res = child.second->solve();
            best_answer = std::max(best_answer, res.first);
            this->status[0] = this->status[0] || res.second.first;
            this->status[1] = this->status[1] || res.second.second;
        }
        if (this->status[0] && this->status[1])
        {
            // found common substring
            if (lengths[this->firstIndex] < (int)(this->length))
            {
                lengths[this->firstIndex] = (int)(this->length);
                lastIndexes[this->firstIndex] = (int)(this->lastIndex);
            }
        }
        return { best_answer, { this->status[0], this->status[1] } };
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
    std::cin.ignore(20, '\n');
    std::cin >> ab_storage >> ca_storage >> bc_storage;
    ab = std::string_view(ab_storage);
    ca = std::string_view(ca_storage);
    bc = std::string_view(bc_storage);

    BCxAB = buildLCSarray(bc, ab);
    CAxBC = buildLCSarray(ca, bc);
    
    Trie trie;
    for (unsigned int i = 0; i <= ab.size(); i++) trie.add(ab.substr(i), 0, i);
    for (unsigned int i = 0; i <= ca.size(); i++) trie.add(ca.substr(i), 1, i);
    lastIndexes.resize(ab.size() + 1, -1);
    lengths.resize(ab.size() + 1, -1);
    trie.solve();
    unsigned int best_answer = 0;
    for (unsigned int i = 0; i <= ab.size(); i++)
    {
        if (lastIndexes[i] != -1)
        {
            for (unsigned int x = 0; x <= bc.size(); ++x)
            {
                unsigned int answer = lengths[i] + CAxBC[lastIndexes[i]][x] + BCxAB[x][i + lengths[i]];
                if (answer > best_answer) best_answer = answer;
            }
        }
    }
    std::cout << best_answer;
}