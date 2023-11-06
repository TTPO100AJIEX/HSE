#include <ios>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

struct Person {
    std::string name;
    std::vector<unsigned int> siblings;
};
std::vector<Person> people;

void print(unsigned int current) {
    std::cout << people[current].name << '\n';
    people[current].name.clear();
    std::sort(people[current].siblings.begin(), people[current].siblings.end(),
              [](unsigned int first, unsigned int second) {
                  return people[first].name < people[second].name;
              });
    for (unsigned int sibling : people[current].siblings) {
        if (!people[sibling].name.empty()) {
            print(sibling);
        }
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n, m;
    std::cin >> n >> m;
    people.resize(n);
    for (unsigned int i = 0; i < n; ++i) {
        unsigned int index;
        std::cin >> index;
        std::cin.ignore(1);
        std::getline(std::cin, people[index].name);
    }
    for (unsigned int i = 0; i < m; ++i) {
        unsigned int from, to;
        std::cin >> from >> to;
        people[from].siblings.push_back(to);
        people[to].siblings.push_back(from);
    }

    unsigned int number;
    std::cin >> number;
    print(number);
    return 0;
}