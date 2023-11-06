#include <ios>
#include <iostream>
#include <string>

struct Trie {
    char letter = 0;
    Trie* left = nullptr;
    Trie* right = nullptr;

    void add(char letter) {
        switch (std::cin.get()) {
            case '\n': {
                this->letter = letter;
                break;
            }
            case '1': {
                if (this->left == nullptr) {
                    this->left = new Trie();
                }
                this->left->add(letter);
                break;
            }
            case '0': {
                if (this->right == nullptr) {
                    this->right = new Trie();
                }
                this->right->add(letter);
                break;
            }
            default: {
                this->add(letter);
            }
        }
    }

    ~Trie() {
        delete this->left;
        delete this->right;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int k, l;
    std::cin >> k >> l;
    std::cin.get();
    std::cin >> std::noskipws;

    Trie root;
    for (unsigned int i = 0; i < k; ++i) {
        root.add(std::cin.get());
    }

    Trie* current = &root;
    for (unsigned int i = 0; i < l; ++i) {
        current = (std::cin.get() == '1') ? current->left : current->right;

        if (current->letter != 0) {
            std::cout << current->letter;
            current = &root;
        }
    }
}