#include <iostream>
#include <vector>
#include <string>

struct Node {
    int key;
    int left;
    int right;
    bool isRed;
};
std::vector<Node> data;
bool check(int index, int num_black) {
    static int num_black_list = -1;
    if (index == -1) {
        if (num_black_list == -1) {
            num_black_list = num_black;
}
        return (num_black == num_black_list);
    }
    const auto& [key, left, right, isRed] = data[index];
    if (left != -1) {
        if (key <= data[left].key) {
            return false;
}
        if (isRed && data[left].isRed) {
            return false;
}
    }
    if (right != -1) {
        if (key >= data[right].key) {
            return false;
}
        if (isRed && data[right].isRed) {
            return false;
}
    }
    return (check(left, num_black + (isRed ? 0 : 1)) && check(right, num_black + (isRed ? 0 : 1)));
}

int main(void) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n;
    std::cin >> n;
    if (n == 0) {
        std::cout << "NO";
        return 0;
    }
    unsigned int root;
    std::cin >> root;
    root--;
    data.resize(n);
    for (unsigned int i = 0; i < n; ++i) {
        unsigned int number;
        int key;
        std::string left, right;
        char color;
        std::cin >> number >> key >> left >> right >> color;
        data[number - 1] = {key, (left == "null" ? 0 : std::stoi(left)) - 1,
                            (right == "null" ? 0 : std::stoi(right)) - 1, color == 'R'};
    }

    if (data[root].isRed) {
        std::cout << "NO";
        return 0;
    }
    if (check(root, 1)) {
        std::cout << "YES";
    } else {
        std::cout << "NO";
}
    return 0;
}