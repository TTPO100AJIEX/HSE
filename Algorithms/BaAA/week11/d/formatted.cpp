#include <ios>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>

class FieldPathfinder {
private:
    struct Vertex {
        int x, y;
    };

    static constexpr unsigned int kDistanceMax = (1 << 30);
    std::vector<std::vector<int> > queue_refs_;
    std::vector<std::vector<unsigned int> > distances_;
    std::vector<std::vector<Vertex> > previous_;

    Vertex to_;
    std::vector<Vertex> to_update_;
    unsigned int getScore(unsigned int i) {
        unsigned int dx = std::abs(this->to_.x - this->to_update_[i].x);
        unsigned int dy = std::abs(this->to_.y - this->to_update_[i].y);
        if (dx > dy) {
            dx = std::max(dx >> 1, dy);
        } else {
            dx = std::max(dx, dy >> 1);
        }
        return this->distances_[this->to_update_[i].x][this->to_update_[i].y] + dx;
    }
    void swap(unsigned int i, unsigned int j) {
        this->queue_refs_[this->to_update_[i].x][this->to_update_[i].y] = j;
        this->queue_refs_[this->to_update_[j].x][this->to_update_[j].y] = i;
        std::swap(this->to_update_[i], this->to_update_[j]);
    }
    void siftUp(unsigned int i) {
        if (i == 0) {
            return;
        }
        const unsigned int parent = (i - 1) >> 1;
        if (this->getScore(parent) > this->getScore(i)) {
            this->swap(i, parent);
            this->siftUp(parent);
        }
    }
    void siftDown(unsigned int i) {
        const unsigned int child_left = (i << 1) + 1, child_right = (i << 1) + 2;
        if (child_left >= this->to_update_.size()) {
            return;
        }
        unsigned int swap_with = this->to_update_.size();
        if (child_right < this->to_update_.size()) {
            // both children exist
            if (this->getScore(child_left) < this->getScore(i) ||
                this->getScore(child_right) < this->getScore(i)) {
                if (this->getScore(child_left) < this->getScore(child_right)) {
                    swap_with = child_left;
                } else {
                    swap_with = child_right;
                }
            }
        } else {
            // only left exists
            if (this->getScore(child_left) < this->getScore(i)) {
                swap_with = child_left;
            }
        }
        if (swap_with < this->to_update_.size()) {
            this->swap(i, swap_with);
            this->siftDown(swap_with);
        }
    }

    void insert(const Vertex& vertex) {
        if (this->queue_refs_[vertex.x][vertex.y] == -1) {
            this->queue_refs_[vertex.x][vertex.y] = this->to_update_.size();
            this->to_update_.push_back(vertex);
            this->siftUp(this->to_update_.size() - 1);
        } else {
            this->siftUp(this->queue_refs_[vertex.x][vertex.y]);
        }
    }
    Vertex extract() {
        Vertex to_return = this->to_update_[0];
        this->queue_refs_[this->to_update_[0].x][this->to_update_[0].y] = -1;
        this->to_update_[0] = this->to_update_[this->to_update_.size() - 1];
        this->queue_refs_[this->to_update_[0].x][this->to_update_[0].y] = 0;
        this->to_update_.pop_back();
        this->siftDown(0);
        return to_return;
    }

    void findPath(const Vertex& from, const Vertex& to) {
        this->to_ = to;
        this->distances_[from.x][from.y] = 0;
        this->previous_[from.x][from.y] = {-1, -1};
        this->insert(from);
        while (distances_[to.x][to.y] ==
               FieldPathfinder::kDistanceMax)  // A path definitely exists; the first update will be
                                               // the answer
        {
            Vertex current = this->extract();
            Vertex siblings[8] = {{current.x + 2, current.y + 1}, {current.x + 2, current.y - 1},
                                  {current.x - 2, current.y + 1}, {current.x - 2, current.y - 1},
                                  {current.x + 1, current.y + 2}, {current.x + 1, current.y - 2},
                                  {current.x - 1, current.y + 2}, {current.x - 1, current.y - 2}};
            for (const Vertex& sibling : siblings) {
                if (sibling.x < 0 || sibling.y < 0 ||
                    sibling.x >= static_cast<int>(this->distances_.size()) ||
                    sibling.y >= static_cast<int>(this->distances_.size()) ||
                    distances_[sibling.x][sibling.y] != FieldPathfinder::kDistanceMax) {
                    continue;
                }
                this->distances_[sibling.x][sibling.y] = this->distances_[current.x][current.y] + 1;
                this->previous_[sibling.x][sibling.y] = current;
                this->insert(sibling);
            }
        }
    }
    void printPath(Vertex current) {
        if (current.x == -1 || current.y == -1) {
            return;
        }
        this->printPath(this->previous_[current.x][current.y]);
        std::cout << current.x + 1 << ' ' << current.y + 1 << '\n';
    }

public:
    FieldPathfinder(unsigned int n) {
        this->queue_refs_.resize(n, std::vector<int>(n, -1));
        this->distances_.resize(n, std::vector<unsigned int>(n, FieldPathfinder::kDistanceMax));
        this->previous_.resize(n, std::vector<Vertex>(n));
    }
    void solve(Vertex from, Vertex to) {
        this->findPath(from, to);
        std::cout << this->distances_[to.x][to.y] << '\n';
        this->printPath(to);
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, x1, y1, x2, y2;
    std::cin >> n >> x1 >> y1 >> x2 >> y2;
    FieldPathfinder solver(n);
    solver.solve({x1 - 1, y1 - 1}, {x2 - 1, y2 - 1});
    return 0;
}