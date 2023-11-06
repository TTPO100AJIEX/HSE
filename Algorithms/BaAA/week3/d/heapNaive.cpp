#include <ios>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

typedef std::vector< std::pair<unsigned int, unsigned int> > Vector;
class Heap
{
private:
    std::vector < std::pair<unsigned int, unsigned int> > data;

    void bubbleUp(unsigned int index)
    {
        if (index == 0) return;
        const unsigned int parent = (index - 1) >> 1;
        if (this->data[parent].second > this->data[index].second)
        {
            std::swap(this->data[parent], this->data[index]);
            this->bubbleUp(parent);
        }
    }
    void bubbleDown(unsigned int index)
    {
        const unsigned int size = this->data.size(), leftIndex = (index << 1) + 1, rightIndex = (index << 1) + 2;
        unsigned int swapWith = size;
        if (leftIndex < size && this->data[index].second > this->data[leftIndex].second) swapWith = leftIndex;
        if (rightIndex < size && this->data[index].second > this->data[rightIndex].second && this->data[leftIndex].second > this->data[rightIndex].second) swapWith = rightIndex;
        if (swapWith == size) return;
        std::swap(this->data[swapWith], this->data[index]);
        this->bubbleDown(swapWith);
    }

public:
    bool empty() const { return this->data.empty(); }

    void insert(const std::pair<unsigned int, unsigned int>& value)
    {
        this->data.push_back(value);
        this->bubbleUp(this->data.size() - 1);
    }
    std::pair<unsigned int, unsigned int> extract()
    {
        std::pair<unsigned int, unsigned int> answer = this->data[0];
        this->data[0] = this->data[this->data.size() - 1];
        this->data.pop_back();
        this->bubbleDown(0);
        return answer;
    }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    Heap sorter;
    unsigned int n; std::cin >> n;
    Vector villages(n);
    for (unsigned int i = 0; i < n; ++i) { villages[i].first = i; std::cin >> villages[i].second; sorter.insert(villages[i]); }
    unsigned int curIndex = 0;
    while (!sorter.empty()) villages[curIndex++] = sorter.extract();
    
    unsigned int m; std::cin >> m;
    Vector shelters(m);
    for (unsigned int i = 0; i < m; ++i) { shelters[i].first = i; std::cin >> shelters[i].second; sorter.insert(shelters[i]); }
    curIndex = 0;
    while (!sorter.empty()) shelters[curIndex++] = sorter.extract();
    shelters.push_back({ 0, 1 << 31 });

    for (Vector::iterator village = villages.begin(), nearest_right = shelters.begin(); village != villages.end(); ++village)
    {
        while (nearest_right->second < village->second) nearest_right++;
        if (nearest_right == shelters.begin()) { village->second = nearest_right->first; continue; }

        if (nearest_right->second + (nearest_right - 1)->second < (village->second << 1)) village->second = nearest_right->first;
        else village->second = (nearest_right - 1)->first;
    }

    shelters.resize(n);
    for (Vector::iterator village = villages.begin(); village != villages.end(); ++village) shelters[village->first] = *village;
    for (Vector::iterator village = shelters.begin(); village != shelters.end(); ++village) std::cout << village->second + 1 << " ";
}