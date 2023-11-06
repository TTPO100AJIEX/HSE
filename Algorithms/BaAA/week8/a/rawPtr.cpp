#include <ios>
#include <iostream>
#include <string>
#include <functional>
#include <set>
#include <iterator>

constexpr unsigned int symbolsSize = 'z' - 'a' + 1;

struct Node
{
    char symbol = 0;
    unsigned int weight;
    Node* left = nullptr;
    Node* right = nullptr;

    Node(char symbol, unsigned int weight) : symbol(symbol), weight(weight) { }
    Node(Node* left, Node* right) : weight(left->weight + right->weight), left(left), right(right) { }

    void buildCodes(std::string* storage, std::string cur = "")
    {
        if (this->symbol == 0)
        {
            this->left->buildCodes(storage, cur + '1');
            this->right->buildCodes(storage, cur + '0');
        }
        else
        {
            if (cur.size() == 0) cur = '1';
            storage[this->symbol - 'a'] = cur;
        }
    }

    ~Node()
    {
        delete this->left;
        delete this->right;
    }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::string data;
    std::cin >> data;

    unsigned int counter[symbolsSize] = { 0 };
    for (char symbol : data) ++counter[symbol - 'a'];
    
    std::function<bool(Node*, Node*)> cmp = [](const Node* first, const Node* second) { return first->weight > second->weight; };
    std::multiset<Node*, std::function<bool(Node*, Node*)>> nodes(cmp);
    for (unsigned int i = 0; i < symbolsSize; ++i)
    {
        if (counter[i] != 0) nodes.insert(new Node(i + 'a', counter[i]));
    }
    std::cout << nodes.size() << ' ';

    while (nodes.size() != 1)
    {
        std::multiset<Node*, std::function<bool(Node*, Node*)>>::iterator last = std::prev(nodes.end()), second = std::prev(last);
        Node* node = new Node(*last, *second);
        nodes.erase(second, nodes.end());
        nodes.insert(node);
    }

    std::string codes[symbolsSize] = { "" };
    Node* root = *(nodes.begin());
    root->buildCodes(codes);
    delete root;
    
    unsigned int encodedSize = 0;
    for (unsigned int i = 0; i < data.size(); ++i) encodedSize += codes[data[i] - 'a'].size();
    std::cout << encodedSize << '\n';

    for (unsigned int i = 0; i < symbolsSize; ++i)
    {
        if (codes[i].size() != 0) std::cout << (char)('a' + i) << ": " << codes[i] << '\n';
    }

    for (unsigned int i = 0; i < data.size(); ++i) std::cout << codes[data[i] - 'a'];
    std::cout << "\n\n";
}