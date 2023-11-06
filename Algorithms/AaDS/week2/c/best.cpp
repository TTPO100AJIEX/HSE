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

    std::vector<std::vector<size_t> > z_functions(n, std::vector<size_t>(n, 0));
    for (size_t string_start = 0; string_start < n; string_start++) {
        std::vector<size_t>& z_function = z_functions[string_start];
        z_function[string_start] = nums_length - string_start - 1;

        size_t solved_left = string_start + 1, solved_right = string_start + 1;
        for (size_t substring_start = string_start + 1; substring_start < n; substring_start++) {
            if (substring_start + 1 <= solved_right) {
                z_function[substring_start] = std::min(solved_right - substring_start + 2,
                                                       z_function[substring_start - solved_left]);
            }
            while (substring_start + z_function[substring_start] < nums_length - 1 &&
                   numbers[string_start + z_function[substring_start] + 1] ==
                       numbers[substring_start + z_function[substring_start] + 1]) {
                z_function[substring_start]++;
            }
            if (substring_start + z_function[substring_start] > solved_right) {
                solved_left = substring_start + 1;
                solved_right = substring_start + z_function[substring_start];
            }
        }
    }

    std::cout << max_ham_dist << "\n";
    for (size_t i = 0; i < answers.size(); i++) {
        bool unique = true;
        for (size_t j = 0; j < i; j++) {
            if (z_functions[n - answers[i] - 1][n - answers[j] - 1] >= n) {
                unique = false;
                break;
            }
        }
        if (!unique) {
            continue;
        }
        std::cout << numbers[n - answers[i]];
        for (size_t j = 1; j < n; j++) {
            std::cout << " " << numbers[n - answers[i] + j];
        }
        std::cout << "; " << answers[i] << "\n";
    }

    return 0;
}