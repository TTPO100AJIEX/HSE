#include <iostream>
#include "RBTree.cpp"

int main(void)
{
    RBTree<int> rb_tree({ 1, 2, 3 });
    rb_tree.insert(1);
    rb_tree.insert(5);
    rb_tree.insert(4);

    std::cout << std::endl;
    return 0;
}