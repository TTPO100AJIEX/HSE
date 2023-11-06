#include <ios>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>

class FieldPathfinder
{
private:
    struct Vertex { int x, y; };

    static constexpr unsigned int distanceMax = (1 << 30);
    std::vector< std::vector<int> > queueRefs;
    std::vector< std::vector<unsigned int> > distances;
    std::vector< std::vector<Vertex> > previous;
    
    Vertex to;
    std::vector<Vertex> to_update;
    unsigned int getScore(unsigned int i)
    {
        unsigned int dx = std::abs(this->to.x - this->to_update[i].x);
        unsigned int dy = std::abs(this->to.y - this->to_update[i].y);
        if (dx > dy) dx = std::max(dx >> 1, dy);
        else dx = std::max(dx, dy >> 1);
        return this->distances[this->to_update[i].x][this->to_update[i].y] + dx;
    }
    void swap(unsigned int i, unsigned int j)
    {
        this->queueRefs[this->to_update[i].x][this->to_update[i].y] = j;
        this->queueRefs[this->to_update[j].x][this->to_update[j].y] = i;
        std::swap(this->to_update[i], this->to_update[j]);
    }
    void siftUp(unsigned int i)
    {
        if (i == 0) return;
        const unsigned int parent = (i - 1) >> 1;
        if (this->getScore(parent) > this->getScore(i))
        {
            this->swap(i, parent);
            this->siftUp(parent);
        }
    }
    void siftDown(unsigned int i)
    {
        const unsigned int childLeft = (i << 1) + 1, childRight = (i << 1) + 2;
        if (childLeft >= this->to_update.size()) return;
        unsigned int swapWith = this->to_update.size();
        if (childRight < this->to_update.size())
        {
            // both children exist
            if (this->getScore(childLeft) < this->getScore(i) || this->getScore(childRight) < this->getScore(i))
            {
                if (this->getScore(childLeft) < this->getScore(childRight)) swapWith = childLeft;
                else swapWith = childRight;
            }
        }
        else
        {
            // only left exists
            if (this->getScore(childLeft) < this->getScore(i)) swapWith = childLeft;
        }
        if (swapWith < this->to_update.size())
        {
            this->swap(i, swapWith);
            this->siftDown(swapWith);
        }
    }

    void insert(const Vertex& vertex)
    {
        if (this->queueRefs[vertex.x][vertex.y] == -1)
        {
            this->queueRefs[vertex.x][vertex.y] = this->to_update.size();
            this->to_update.push_back(vertex);
            this->siftUp(this->to_update.size() - 1);
        }
        else
        {
            this->siftUp(this->queueRefs[vertex.x][vertex.y]);
        }
    }
    Vertex extract()
    {
        Vertex to_return = this->to_update[0];
        this->queueRefs[this->to_update[0].x][this->to_update[0].y] = -1;
        this->to_update[0] = this->to_update[this->to_update.size() - 1];
        this->queueRefs[this->to_update[0].x][this->to_update[0].y] = 0;
        this->to_update.pop_back();
        this->siftDown(0);
        return to_return;
    }

    void findPath(const Vertex& from, const Vertex& to)
    {
        this->to = to;
        this->distances[from.x][from.y] = 0;
        this->previous[from.x][from.y] = { -1, -1 };
        this->insert(from);
        while (distances[to.x][to.y] == FieldPathfinder::distanceMax) // A path definitely exists; the first update will be the answer
        {
            Vertex current = this->extract();
            Vertex siblings[8] = {
                { current.x + 2, current.y + 1 },
                { current.x + 2, current.y - 1 },
                { current.x - 2, current.y + 1 },
                { current.x - 2, current.y - 1 },
                { current.x + 1, current.y + 2 },
                { current.x + 1, current.y - 2 },
                { current.x - 1, current.y + 2 },
                { current.x - 1, current.y - 2 }
            };
            for (const Vertex& sibling : siblings)
            {
                if (sibling.x < 0 || sibling.y < 0 ||
                    sibling.x >= (int)(this->distances.size()) || sibling.y >= (int)(this->distances.size()) ||
                    distances[sibling.x][sibling.y] != FieldPathfinder::distanceMax)
                {
                    continue;
                }
                this->distances[sibling.x][sibling.y] = this->distances[current.x][current.y] + 1;
                this->previous[sibling.x][sibling.y] = current;
                this->insert(sibling);
            }
        }
    }
    void printPath(Vertex current)
    {
        if (current.x == -1 || current.y == -1) return;
        this->printPath(this->previous[current.x][current.y]);
        std::cout << current.x + 1 << ' ' << current.y + 1 << '\n';
    }

public:
    FieldPathfinder(unsigned int n)
    {
        this->queueRefs.resize(n, std::vector<int>(n, -1));
        this->distances.resize(n, std::vector<unsigned int>(n, FieldPathfinder::distanceMax));
        this->previous.resize(n, std::vector<Vertex>(n));
    }
    void solve(Vertex from, Vertex to)
    {
        this->findPath(from, to);
        std::cout << this->distances[to.x][to.y] << '\n';
        this->printPath(to);
    }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, x1, y1, x2, y2;
    std::cin >> n >> x1 >> y1 >> x2 >> y2;
    FieldPathfinder solver(n);
    solver.solve({ x1 - 1, y1 - 1 }, { x2 - 1, y2 - 1 });
    return 0;
}