#include <ios>
#include <iostream>
#include <string>
#include <memory>
#include <functional>
#include <set>
#include <iterator>

constexpr unsigned int kSymbolsSize = 'z' - 'a' + 1;

struct Node {
    char symbol = 0;
    unsigned int weight;
    std::shared_ptr<Node> left = nullptr;
    std::shared_ptr<Node> right = nullptr;

    Node(char symbol, unsigned int weight) : symbol(symbol), weight(weight) {
    }
    Node(std::shared_ptr<Node> left, std::shared_ptr<Node> right)
        : weight(left->weight + right->weight), left(left), right(right) {
    }

    void buildCodes(std::string* storage, std::string cur = "") {
        if (this->symbol == 0) {
            this->left->buildCodes(storage, cur + '1');
            this->right->buildCodes(storage, cur + '0');
        } else {
            if (cur.empty()) {
                cur = '1';
            }
            storage[this->symbol - 'a'] = cur;
        }
    }
};

typedef std::function<bool(const std::shared_ptr<Node>&, const std::shared_ptr<Node>&)> Comparator;
typedef std::multiset<std::shared_ptr<Node>, Comparator> SetType;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::string data;
    std::cin >> data;

    unsigned int counter[kSymbolsSize] = {0};
    for (char symbol : data) {
        ++counter[symbol - 'a'];
    }

    SetType nodes([](const std::shared_ptr<Node>& first, const std::shared_ptr<Node>& second) {
        return first->weight > second->weight;
    });
    for (unsigned int i = 0; i < kSymbolsSize; ++i) {
        if (counter[i] != 0) {
            nodes.insert(std::make_shared<Node>(i + 'a', counter[i]));
        }
    }
    std::cout << nodes.size() << ' ';

    while (nodes.size() != 1) {
        SetType::iterator last = std::prev(nodes.end()), second = std::prev(last);
        std::shared_ptr<Node> node = std::make_shared<Node>(*last, *second);
        nodes.erase(second, nodes.end());
        nodes.insert(node);
    }

    std::string codes[kSymbolsSize] = {""};
    (*(nodes.begin()))->buildCodes(codes);

    unsigned int encoded_size = 0;
    for (unsigned int i = 0; i < data.size(); ++i) {
        encoded_size += codes[data[i] - 'a'].size();
    }
    std::cout << encoded_size << '\n';

    for (unsigned int i = 0; i < kSymbolsSize; ++i) {
        if (!codes[i].empty()) {
            std::cout << static_cast<char>('a' + i) << ": " << codes[i] << '\n';
        }
    }

    for (unsigned int i = 0; i < data.size(); ++i) {
        std::cout << codes[data[i] - 'a'];
    }
    std::cout << "\n\n";
}