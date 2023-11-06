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
std::vector<std::vector<unsigned int> > b_cx_ab, c_ax_bc;
std::vector<int> last_indexes;
std::vector<int> lengths;

std::vector<std::vector<unsigned int> > buildLCSarray(std::string_view prefixes,
                                                      std::string_view suffixes) {
    std::vector<std::vector<unsigned int> > dp_suf_suf(
        prefixes.size() + 1, std::vector<unsigned int>(suffixes.size() + 1, 0));
    for (int i = prefixes.size() - 1; i >= 0; --i) {
        for (int j = suffixes.size() - 1; j >= 0; --j) {
            if (prefixes[i] == suffixes[j]) {
                dp_suf_suf[i][j] = dp_suf_suf[i + 1][j + 1] + 1;
            }
        }
    }

    std::vector<std::vector<unsigned int> > dp_pref_suf(
        prefixes.size() + 1, std::vector<unsigned int>(suffixes.size() + 1, 0));
    for (unsigned int j = 0; j <= suffixes.size(); ++j) {
        unsigned int pref_len = 0;
        for (unsigned int i = 0; i <= prefixes.size(); ++i) {
            while (pref_len <= i && pref_len + dp_suf_suf[pref_len][j] <= i) {
                ++pref_len;
            }
            dp_pref_suf[i][j] = i - pref_len + 1;
        }
    }

    std::vector<std::vector<unsigned int> > answer(
        prefixes.size() + 1, std::vector<unsigned int>(suffixes.size() + 1, 0));
    for (unsigned int i = 1; i <= prefixes.size(); ++i) {
        for (int j = suffixes.size() - 1; j >= 0; --j) {
            answer[i][j] =
                std::max(std::max(answer[i - 1][j], answer[i][j + 1]), dp_pref_suf[i - 1][j]);
        }
    }
    return answer;
}

class Trie {
private:
    Trie* parent_ = nullptr;
    std::map<char, Trie*> children_;
    bool status_[2] = {false, false};
    unsigned int first_index_ = 3000;
    unsigned int last_index_ = 0;
    unsigned int length_ = 0;

    Trie* getChild(char symbol) const {
        std::map<char, Trie*>::const_iterator iter = this->children_.find(symbol);
        if (iter == this->children_.end()) {
            return nullptr;
        }
        return iter->second;
    }

public:
    Trie(Trie* parent = nullptr) : parent_(parent){};

    void add(std::string_view data, unsigned int status, unsigned int start_index) {
        if (status == 0) {
            this->first_index_ = std::min(this->first_index_, start_index);
        }
        if (status == 1) {
            this->last_index_ = std::max(this->last_index_, start_index);
        }
        if (data.empty()) {
            this->status_[status] = true;
            return;
        }
        Trie* child = this->getChild(data[0]);
        if (!child) {
            child = this->children_.insert_or_assign(data[0], new Trie(this)).first->second;
            child->length_ = this->length_ + 1;
        }
        child->add(data.substr(1), status, start_index);
    }

    std::pair<unsigned int, std::pair<bool, bool> > solve() {
        unsigned int best_answer = 0;
        for (std::pair<char, Trie*> child : this->children_) {
            std::pair<unsigned int, std::pair<bool, bool> > res = child.second->solve();
            best_answer = std::max(best_answer, res.first);
            this->status_[0] = this->status_[0] || res.second.first;
            this->status_[1] = this->status_[1] || res.second.second;
        }
        if (this->status_[0] && this->status_[1]) {
            // found common substring
            if (lengths[this->first_index_] < static_cast<int>(this->length_)) {
                lengths[this->first_index_] = static_cast<int>(this->length_);
                last_indexes[this->first_index_] = static_cast<int>(this->last_index_);
            }
        }
        return {best_answer, {this->status_[0], this->status_[1]}};
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
    std::cin.ignore(20, '\n');
    std::cin >> ab_storage >> ca_storage >> bc_storage;
    ab = std::string_view(ab_storage);
    ca = std::string_view(ca_storage);
    bc = std::string_view(bc_storage);

    b_cx_ab = buildLCSarray(bc, ab);
    c_ax_bc = buildLCSarray(ca, bc);

    Trie trie;
    for (unsigned int i = 0; i <= ab.size(); i++) {
        trie.add(ab.substr(i), 0, i);
    }
    for (unsigned int i = 0; i <= ca.size(); i++) {
        trie.add(ca.substr(i), 1, i);
    }
    last_indexes.resize(ab.size() + 1, -1);
    lengths.resize(ab.size() + 1, -1);
    trie.solve();
    unsigned int best_answer = 0;
    for (unsigned int i = 0; i <= ab.size(); i++) {
        if (last_indexes[i] != -1) {
            for (unsigned int x = 0; x <= bc.size(); ++x) {
                unsigned int answer =
                    lengths[i] + c_ax_bc[last_indexes[i]][x] + b_cx_ab[x][i + lengths[i]];
                if (answer > best_answer) {
                    best_answer = answer;
                }
            }
        }
    }
    std::cout << best_answer;
}