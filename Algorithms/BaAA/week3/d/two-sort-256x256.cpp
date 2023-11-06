#include <ios>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

typedef std::vector< std::pair<unsigned int, unsigned int> > Vector;
Vector res;
void radixSort(Vector& data)
{
    for (unsigned int offset = 0; offset < 32; offset += 16)
    {
        unsigned int counter[65536] = { 0 };
        for (unsigned int i = 0; i < data.size(); ++i) ++counter[(data[i].second >> offset) & 65535];
        for (unsigned int i = 1; i < 65536; ++i) counter[i] += counter[i - 1];
        for (int i = data.size() - 1; i >= 0; --i) res[--counter[(data[i].second >> offset) & 65535]] = data[i];
        std::swap(data, res);
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n; std::cin >> n;
    Vector villages(n);
    for (unsigned int i = 0; i < n; ++i) { villages[i].first = i; std::cin >> villages[i].second; }
    
    unsigned int m; std::cin >> m;
    Vector shelters(m);
    for (unsigned int i = 0; i < m; ++i) { shelters[i].first = i; std::cin >> shelters[i].second; }

    if (n > m)
    {
        res.resize(n); radixSort(villages);
        res.resize(m); radixSort(shelters);
    }
    else
    {
        res.resize(m); radixSort(shelters);
        res.resize(n); radixSort(villages);
    }
    shelters.push_back({ 0, 1 << 31 });

    for (Vector::iterator village = villages.begin(), nearest_right = shelters.begin(); village != villages.end(); ++village)
    {
        while (nearest_right->second < village->second) nearest_right++;
        if (nearest_right == shelters.begin()) { village->second = nearest_right->first; continue; }

        if (nearest_right->second + (nearest_right - 1)->second < (village->second << 1)) village->second = nearest_right->first;
        else village->second = (nearest_right - 1)->first;
    }

    res.resize(n);
    for (Vector::iterator village = villages.begin(); village != villages.end(); ++village) res[village->first] = *village;
    for (Vector::iterator village = res.begin(); village != res.end(); ++village) std::cout << village->second + 1 << " ";
}