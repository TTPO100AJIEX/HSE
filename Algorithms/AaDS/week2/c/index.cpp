#include <iostream>
#include <algorithm>
#include <vector>

size_t hamDist(int* array1, int* array2, size_t size) {
    size_t answer = 0;
    for (size_t i = 0; i < size; i++, array1++, array2++) {
        answer += (*array1 != *array2);
    }
    return answer;
}

int main(void) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    size_t n;
    std::cin >> n;
    size_t nums_length = (n << 1);
    std::vector<int> numbers(nums_length);
    for (size_t i = 0; i < n; i++) {
        std::cin >> numbers[i];
    }
    for (size_t i = n; i < nums_length; i++) {
        numbers[i] = numbers[i - n];
    }

    std::vector<int> answers = {0};
    size_t max_ham_dist = 0;
    for (size_t i = 1; i < n; i++) {
        size_t dist = hamDist(&numbers[0], &numbers[n - i], n);
        if (dist == 0) {
            break;
        }
        if (dist > max_ham_dist) {
            answers.clear();
            max_ham_dist = dist;
        }
        if (dist == max_ham_dist) {
            answers.push_back(i);
        }
    }

    std::cout << max_ham_dist << "\n";
    for (size_t i = 0; i < answers.size(); i++) {
        std::cout << numbers[n - answers[i]];
        for (size_t j = 1; j < n; j++) {
            std::cout << " " << numbers[n - answers[i] + j];
        }
        std::cout << "; " << answers[i] << "\n";
    }

    return 0;
}