#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <map>
#include "Map.cpp"

int main(void)
{
    Map<int, int> test;
    test.insert(1, 2);
    std::cout << test.find(1)->second << std::endl;
    test.insert(1, 3);
    std::cout << (*test.find(1)).second << std::endl;
    test.erase(1);
    std::cout << test.size() << std::endl;
    std::cout << test.size() << std::endl;
    
    Map<int, int> test2(test);
    Map<int, int> test3 = test;
    test3.insert(1, 2);
    const std::pair <int, int>* storage;
    {
        Map<int, int>::Iterator now = test3.begin();
        storage = now.operator->();
    }
    std::cout << storage->first << std::endl;
    /*std::map<int, int> test;
    test.insert({ 1, 2 });
    std::pair <const int, int>* storage;
    {
        std::map<int, int>::iterator now = test.begin();
        storage = now.operator->();
    }
    std::cout << storage->first << std::endl;*/




    /*
    std::vector <int> A = { 98, 41, 81, 45, 32, 7, 35, 29, 47, 13, 83, 89, 77, 42, 94, 74, 90, 65, 79, 72, 48, 5, 60, 61, 34, 67, 52, 3, 64, 63, 95, 10, 68, 23, 43, 38, 36, 16, 20, 93, 2 };
    std::random_device rd;
    A = { };
    std::mt19937 g(rd());
    std::uniform_int_distribution<std::mt19937::result_type> random(0, 2e9);
    for (int i = 0; i < 1e6; i++)
    {
        A.push_back(random(g) - 1e9);
    }
    //std::vector <int> A;
    //for (size_t i = 0; i < 7; i++) A.push_back(data[i]);
    RBTree<int> rb_tree;
    for (size_t i = 0; i < A.size(); i++)
    {
        // std::cout << "Inserted " << A[i] << std::endl;

        rb_tree.insert(A[i]);

        // rb_tree.output();
        // std::cout << std::endl;
    }
    std::shuffle(A.begin(), A.end(), g);
    RBTree<int> rb_tree2(rb_tree);
    RBTree<int> rb_tree3 = rb_tree;
    int prev = -1e9 - 1;
    for (RBTree<int>::Iterator now = rb_tree3.begin(); now != rb_tree3.end(); now++)
    {
        if (*now <= prev)
        {
            std::cout << "ERROR";
        }
        prev = *now;
    }
    std::cout << std::endl;
    std::cout << rb_tree2.size() << " " << rb_tree.size() << std::endl;
    for (size_t i = 0; i < A.size(); i++)
    {
        // std::cout << "Erased " << A[i] << std::endl;

        rb_tree2.erase(A[i]);
        if (rb_tree2.size() % 1000 == 0)
        {
            if (rb_tree2.size() % 10000 == 0) std::cout << rb_tree2.size() << std::endl;
            prev = -1e9 - 1;
            for (RBTree<int>::Iterator now = rb_tree2.begin(); now != rb_tree2.end(); now++)
            {
                if (*now <= prev)
                {
                    std::cout << "ERROR";
                }
                prev = *now;
            }
        }

        // rb_tree.output();
        // std::cout << std::endl;
    }

    std::cout << rb_tree2.size() << " " << rb_tree.size() << std::endl;
*/
    return 0;
}