#include <ios>
#include <iostream>
#include <vector>
#include <utility>
#include <memory>

class Place
{
public:
    unsigned int index;
    unsigned int coord;
    Place(unsigned int index, unsigned int coord) : index(index), coord(coord) { }
    virtual bool isLiving() const = 0;
    virtual bool isBombing() const = 0;
    virtual ~Place() = default;
};

class Bomb : public Place
{
public:
    Bomb(unsigned int index, unsigned int coord) : Place(index, coord) { }
    virtual bool isLiving() const override { return false; }
    virtual bool isBombing() const override { return true; }
    virtual ~Bomb() = default;
};

class Live : public Place
{
public:
    Live(unsigned int index, unsigned int coord) : Place(index, coord) { }
    virtual bool isLiving() const override { return true; }
    virtual bool isBombing() const override { return false; }
    virtual ~Live() = default;

    Bomb* leftBombing = nullptr;
    Bomb* rightBombing = nullptr;
};

class Heap
{
private:
    std::vector < std::unique_ptr<Place> > data_;

    void bubbleUp(unsigned int index)
    {
        if (index == 0) return;
        const unsigned int parent = (index - 1) >> 1;
        if (this->data_[parent]->coord > this->data_[index]->coord)
        {
            std::swap(this->data_[parent], this->data_[index]);
            this->bubbleUp(parent);
        }
    }
    void bubbleDown(unsigned int index)
    {
        const unsigned int size = this->data_.size(), leftIndex = (index << 1) + 1, rightIndex = (index << 1) + 2;
        unsigned int swapWith = size;
        if (leftIndex < size && this->data_[index]->coord > this->data_[leftIndex]->coord) swapWith = leftIndex;
        if (rightIndex < size && this->data_[index]->coord > this->data_[rightIndex]->coord && this->data_[leftIndex]->coord > this->data_[rightIndex]->coord) swapWith = rightIndex;
        if (swapWith == size) return;
        std::swap(this->data_[swapWith], this->data_[index]);
        this->bubbleDown(swapWith);
    }

public:
    size_t size() const { return this->data_.size(); }
    bool empty() const { return this->data_.empty(); }

    void insert(std::unique_ptr<Place> value)
    {
        this->data_.push_back(std::move(value));
        this->bubbleUp(this->data_.size() - 1);
    }
    std::unique_ptr<Place> extract()
    {
        std::unique_ptr<Place> answer = std::move(this->data_[0]);
        this->data_[0] = std::move(this->data_[this->data_.size() - 1]);
        this->data_.pop_back();
        this->bubbleDown(0);
        return answer;
    }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    Heap sorter;
    unsigned int n;
    std::cin >> n;
    for (unsigned int i = 0; i < n; i++)
    {
        unsigned int coord; std::cin >> coord;
        sorter.insert(std::make_unique<Live>(i, coord));
    }
    std::cin >> n;
    for (unsigned int i = 0; i < n; i++)
    {
        unsigned int coord; std::cin >> coord;
        sorter.insert(std::make_unique<Bomb>(i, coord));
    }

    std::vector< std::unique_ptr<Place> > places;
    places.reserve(sorter.size());
    while (!sorter.empty()) places.push_back(sorter.extract());

    Bomb* current = nullptr;
    for (unsigned int i = 0; i < places.size(); i++)
    {
        if (places[i]->isBombing()) current = static_cast<Bomb*>(places[i].get());
        else (static_cast<Live*>(places[i].get()))->leftBombing = current;
    }
    current = nullptr;
    for (int i = places.size() - 1; i >= 0; i--)
    {
        if (places[i]->isBombing()) current = static_cast<Bomb*>(places[i].get());
        else (static_cast<Live*>(places[i].get()))->rightBombing = current;
    }

    for (unsigned int i = 0; i < places.size(); i++)
    {
        if (places[i]->isBombing()) continue;
        Live* obj = static_cast<Live*>(places[i].get());
        unsigned int leftDist = (1 << 31), rightDist = (1 << 31);
        if (obj->leftBombing) leftDist = obj->coord - obj->leftBombing->coord;
        if (obj->rightBombing) rightDist = obj->rightBombing->coord - obj->coord;
        std::cout << (leftDist <= rightDist ? obj->leftBombing->index : obj->rightBombing->index) + 1 << ' ';
    }
}