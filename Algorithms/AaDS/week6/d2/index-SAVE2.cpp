#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include "RBTree.cpp"

struct op
{
    enum class Type { INSERT, ERASE };
    Type type;
    int value;
};
void all(const std::vector<op>& used)
{
    if (used.size() == 7)
    {
        RBTree<int> test;
        for (size_t i = 0; i < used.size(); i++)
        {
            switch (used[i].type)
            {
                case op::Type::INSERT: { test.insert(used[i].value); break; }
                case op::Type::ERASE: { test.erase(used[i].value); break; }
            }
        }
        return;
    }
    for (int i = 1; i <= 7; i++)
    {
        std::vector<op> n = used;
        n.push_back({ op::Type::INSERT, i });
        all(n);
        n = used;
        n.push_back({ op::Type::ERASE, i });
        all(n);
    }
}

int main(void)
{
    std::vector<op> empty;
    all(empty);
    return 0;
}