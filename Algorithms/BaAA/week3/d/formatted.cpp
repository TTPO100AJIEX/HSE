#include <ios>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

#define COMP                                                  \
    [](const std::pair<unsigned int, unsigned int>& first,    \
       const std::pair<unsigned int, unsigned int>& second) { \
        return first.second < second.second;                  \
    }
void heapSort(std::vector<std::pair<unsigned int, unsigned int> >& data) {
    std::make_heap(data.begin(), data.end(), COMP);
    std::vector<std::pair<unsigned int, unsigned int> >::iterator end = data.end();
    while (end != data.begin() + 1) {
        std::pop_heap(data.begin(), end, COMP);
        end--;
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n;
    std::cin >> n;
    std::vector<std::pair<unsigned int, unsigned int> > villages(n);
    for (unsigned int i = 0; i < n; ++i) {
        villages[i].first = i;
        std::cin >> villages[i].second;
    }
    heapSort(villages);

    unsigned int m;
    std::cin >> m;
    std::vector<std::pair<unsigned int, unsigned int> > shelters(m);
    for (unsigned int i = 0; i < m; ++i) {
        shelters[i].first = i;
        std::cin >> shelters[i].second;
    }
    heapSort(shelters);
    shelters.push_back({0, 1 << 31});

    for (std::vector<std::pair<unsigned int, unsigned int> >::iterator
             village = villages.begin(),
             nearest_right = shelters.begin();
         village != villages.end(); ++village) {
        while (nearest_right->second < village->second) {
            nearest_right++;
        }
        if (nearest_right == shelters.begin()) {
            village->second = nearest_right->first;
            continue;
        }

        if (nearest_right->second + (nearest_right - 1)->second < (village->second << 1)) {
            village->second = nearest_right->first;
        } else {
            village->second = (nearest_right - 1)->first;
        }
    }

    shelters.resize(n);
    for (std::vector<std::pair<unsigned int, unsigned int> >::iterator village = villages.begin();
         village != villages.end(); ++village) {
        shelters[village->first] = *village;
    }
    for (std::vector<std::pair<unsigned int, unsigned int> >::iterator village = shelters.begin();
         village != shelters.end(); ++village) {
        std::cout << village->second + 1 << " ";
    }
}