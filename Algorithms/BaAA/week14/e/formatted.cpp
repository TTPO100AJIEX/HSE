#include <ios>
#include <iostream>
#include <climits>
#include <vector>
#include <utility>
#include <functional>

class PriorityQueue {
private:
    using Comparator = std::function<bool(unsigned int, unsigned int)>;
    Comparator comparator_;
    std::vector<unsigned int> storage_;
    std::vector<unsigned int> index_;

    void swap(unsigned int first, unsigned int second) {
        std::swap(this->index_[this->storage_[first]], this->index_[this->storage_[second]]);
        std::swap(this->storage_[first], this->storage_[second]);
    }

    void siftUp(unsigned int index) {
        if (index == 0) {
            return;
        }
        const unsigned int parent = (index - 1) >> 1;
        if (!this->comparator_(this->storage_[parent], this->storage_[index])) {
            this->swap(index, parent);
            this->siftUp(parent);
        }
    }

    void siftDown(unsigned int index) {
        const unsigned int left = (index << 1) + 1, right = (index << 1) + 2;
        if (left >= this->storage_.size()) {
            return;
        }
        unsigned int to_swap = this->storage_.size();
        if (right < this->storage_.size()) {
            // both children exist
            if (!this->comparator_(this->storage_[index], this->storage_[left]) ||
                !this->comparator_(this->storage_[index], this->storage_[right])) {
                to_swap =
                    !this->comparator_(this->storage_[right], this->storage_[left]) ? left : right;
            }
        } else {
            // only left exists
            if (!this->comparator_(this->storage_[index], this->storage_[left])) {
                to_swap = left;
            }
        }
        if (to_swap != this->storage_.size()) {
            this->swap(index, to_swap);
            this->siftDown(to_swap);
        }
    }

public:
    PriorityQueue(unsigned int max_key, Comparator comparator) : comparator_(comparator) {
        this->index_.resize(max_key, UINT_MAX);
    }

    void insert(unsigned int item) {
        if (this->index_[item] == UINT_MAX) {
            this->index_[item] = this->storage_.size();
            this->storage_.push_back(item);
        }
        this->siftUp(this->index_[item]);
    }

    unsigned int extract() {
        unsigned int to_return = this->storage_[0];
        this->index_[to_return] = UINT_MAX;
        this->storage_[0] = this->storage_[this->storage_.size() - 1];
        this->index_[this->storage_[0]] = 0;
        this->storage_.pop_back();
        this->siftDown(0);
        return to_return;
    }

    bool empty() {
        return this->storage_.empty();
    }
};

unsigned int getSumDigits(unsigned int number) {
    unsigned int answer = 0;
    while (number > 0) {
        answer += (number % 10);
        number /= 10;
    }
    return answer;
}

void printPath(const std::vector<unsigned int>& parents, unsigned int vertex,
               unsigned int dist = 0) {
    if (parents[vertex] == 0) {
        std::cout << dist << '\n';
        return;
    }
    printPath(parents, parents[vertex], dist + 1);
    int16_t operation = vertex - parents[vertex];
    if (operation > 0) {
        std::cout << '+';
    }
    std::cout << operation << '\n';
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n, a, b;
    std::cin >> n >> a >> b;
    ++n;

    constexpr unsigned int kInf = UINT_MAX;
    std::vector<unsigned int> parents(n, 0);
    std::vector<unsigned int> distances(n, kInf);
    distances[a] = 0;

    PriorityQueue to_update(
        n, [&](unsigned int a, unsigned int b) { return distances[a] < distances[b]; });
    to_update.insert(a);
    while (!to_update.empty()) {
        const unsigned int from = to_update.extract();
        const unsigned int current_distance = distances[from];

        unsigned int prefix = from;
        while (prefix > 0) {
            unsigned int suffix = prefix;
            unsigned int power10 = 1;
            do {
                unsigned int y = suffix % (power10 *= 10);
                unsigned int attempt_distance = current_distance + getSumDigits(y);
                if (from + y < n && attempt_distance < distances[from + y]) {
                    distances[from + y] = attempt_distance;
                    parents[from + y] = from;
                    to_update.insert(from + y);
                }
                if (from != y && attempt_distance < distances[from - y]) {
                    distances[from - y] = attempt_distance;
                    parents[from - y] = from;
                    to_update.insert(from - y);
                }
            } while (suffix % power10 != prefix);
            prefix /= 10;
        }
    }

    if (distances[b] != kInf) {
        std::cout << distances[b] << '\n';
        printPath(parents, b);
        return 0;
    }

    std::cout << -1;
}