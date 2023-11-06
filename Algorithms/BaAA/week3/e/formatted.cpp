#include <ios>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

void heapSort(std::vector<std::pair<unsigned int, int> >& data) {
    std::make_heap(data.begin(), data.end());
    std::vector<std::pair<unsigned int, int> >::iterator end = data.end();
    while (end != data.begin() + 1) {
        std::pop_heap(data.begin(), end);
        end--;
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n;
    std::cin >> n;
    std::vector<std::pair<unsigned int, int> > events(n << 1);
    for (std::vector<std::pair<unsigned int, int> >::iterator now = events.begin();
         now != events.end(); now += 2) {
        std::vector<std::pair<unsigned int, int> >::iterator next = now + 1;
        std::cin >> now->first >> next->first;
        now->second = 1;
        next->second = -1;
        next->first++;
    }
    heapSort(events);

    std::vector<std::pair<unsigned int, int> > points;
    points.emplace_back(0, 0);
    for (std::vector<std::pair<unsigned int, int> >::iterator event = events.begin();
         event != events.end(); ++event) {
        if (points.back().first != event->first) {
            points.emplace_back(event->first, points.back().second);
        }
        points.back().second += event->second;
    }

    char command;
    std::cin >> command;
    while (command != '!') {
        unsigned int time;
        std::cin >> time;
        std::vector<std::pair<unsigned int, int> >::iterator point =
            std::upper_bound(
                points.begin(), points.end(), time,
                [](const unsigned int value, const std::pair<unsigned int, int>& point) {
                    return value < point.first;
                }) -
            1;
        std::cout << "! " << point->second << std::endl;
        std::cin >> command;
    }
}