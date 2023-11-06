#include <ios>
#include <iostream>
#include <climits>
#include <vector>
#include <utility>
#include <functional>

class PriorityQueue
{
private:
    using Comparator = std::function<bool(unsigned int, unsigned int)>;
    Comparator comparator;
    std::vector<unsigned int> storage;
    std::vector<unsigned int> index;

    void swap(unsigned int first, unsigned int second)
    {
        std::swap(this->index[this->storage[first]], this->index[this->storage[second]]);
        std::swap(this->storage[first], this->storage[second]);
    }
    
    void siftUp(unsigned int index)
    {
        if (index == 0) return;
        const unsigned int parent = (index - 1) >> 1;
        if (this->comparator(this->storage[index], this->storage[parent]))
        {
            this->swap(index, parent);
            this->siftUp(parent);
        }
    }

    void siftDown(unsigned int index)
    {
        const unsigned int left = (index << 1) + 1, right = (index << 1) + 2;
        if (left >= this->storage.size()) return;
        unsigned int to_swap = this->storage.size();
        if (right < this->storage.size())
        {
            // both children exist
            if (this->comparator(this->storage[left], this->storage[index]) || this->comparator(this->storage[right], this->storage[index]))
                to_swap = this->comparator(this->storage[left], this->storage[right]) ? left : right;
        }
        else
        {
            // only left exists
            if (this->comparator(this->storage[left], this->storage[index])) to_swap = left;
        }
        if (to_swap != this->storage.size())
        {
            this->swap(index, to_swap);
            this->siftDown(to_swap);
        }
    }

public:
    PriorityQueue(unsigned int maxKey, Comparator comparator) : comparator(comparator) { this->index.resize(maxKey, UINT_MAX); }

    void insert(unsigned int item)
    {
        if (this->index[item] == UINT_MAX)
        {
            this->index[item] = this->storage.size();
            this->storage.push_back(item);
        }
        this->siftUp(this->index[item]);
    }

    unsigned int extract()
    {
        unsigned int to_return = this->storage[0];
        this->index[to_return] = UINT_MAX;
        this->storage[0] = this->storage[this->storage.size() - 1];
        this->index[this->storage[0]] = 0;
        this->storage.pop_back();
        this->siftDown(0);
        return to_return;
    }

    bool empty() { return this->storage.empty(); }
};

unsigned int getSumDigits(unsigned int number)
{
    unsigned int answer = 0;
    while (number > 0)
    {
        answer += (number % 10);
        number /= 10;
    }
    return answer;
}

void printPath(const std::vector<unsigned int>& parents, unsigned int vertex, unsigned int dist = 0)
{
    if (parents[vertex] == 0) { std::cout << dist << '\n'; return; }
    printPath(parents, parents[vertex], dist + 1);
    int16_t operation = vertex - parents[vertex];
    if (operation > 0) std::cout << '+';
    std::cout << operation << '\n';
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n, a, b;
    std::cin >> n >> a >> b;
    ++n;

    constexpr unsigned int INF = UINT_MAX;
    std::vector<unsigned int> parents(n, 0);
    std::vector<unsigned int> distances(n, INF);
    distances[a] = 0;

    PriorityQueue to_update(n, [&](unsigned int a, unsigned int b) { return distances[a] < distances[b]; });
    to_update.insert(a);
    while (!to_update.empty())
    {
        const unsigned int from = to_update.extract();
        const unsigned int current_distance = distances[from];
        std::cout << from << std::endl;
        /*
        5
10
20
9
11
12
18
13
40
22
17
14
19
24
15
16
44
36
26
80
28
39
33
48
30
32
34
42
60
27
38
35
31
88
72
52
29
37
66
25
41
64
46
70
54
50
74
23
68
62
84
56
21
57
47
78
45
58
59
55
100
49
82
79
65
96
90
99
43
67
77
61
81
51
63
76
53
92
71
85
94
86
69
73
83
87
89
98
75
93
91
97
95
29
8*/

        unsigned int prefix = from;
        while (prefix > 0)
        {
            unsigned int suffix = prefix;
            unsigned int power10 = 1;
            do
            {
                unsigned int y = suffix % (power10 *= 10);
                unsigned int attempt_distance = current_distance + getSumDigits(y);
                if (from + y < n && attempt_distance < distances[from + y])
                {
                    distances[from + y] = attempt_distance;
                    parents[from + y] = from;
                    to_update.insert(from + y);
                }
                if (from != y && attempt_distance < distances[from - y])
                {
                    distances[from - y] = attempt_distance;
                    parents[from - y] = from;
                    to_update.insert(from - y);
                }
            } while (suffix % power10 != prefix);
            prefix /= 10;
        }
    }

    if (distances[b] != INF)
    {
        std::cout << distances[b] << '\n';
        printPath(parents, b);
        return 0;
    }

    std::cout << -1;
}