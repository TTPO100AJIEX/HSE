#include <list>
using std::list;

list<int> merge(const list<int>& first, const list<int>& second)
{
    list<int> res;
    list<int>::const_iterator first_iter = first.begin(), second_iter = second.begin();

    if (*(first.rbegin()) < *(second.rbegin()))
    {
        while (first_iter != first.end())
        {
            if (*first_iter < *second_iter) res.push_back(*(first_iter++));
            else res.push_back(*(second_iter++));
        }
        while (second_iter != second.end()) res.push_back(*(second_iter++));
    }
    else
    {
        while (second_iter != second.end())
        {
            if (*first_iter < *second_iter) res.push_back(*(first_iter++));
            else res.push_back(*(second_iter++));
        }
        while (first_iter != first.end()) res.push_back(*(first_iter++));
    }

    return res;
}