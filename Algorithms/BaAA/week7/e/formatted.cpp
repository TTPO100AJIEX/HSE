#include <ios>
#include <iostream>
#include <utility>
#include <vector>
#include <string>

bool accepts(const std::string& s1, const std::string& s2, unsigned int max_errors,
             unsigned int index1, unsigned int index2, unsigned int cur_errors) {
    if (cur_errors > max_errors) {
        return false;
    }
    if (index1 == s1.size()) {
        return (cur_errors + s2.size() <= max_errors + index2);  // Delete remaining
    }
    if (index2 == s2.size()) {
        return (cur_errors + s1.size() <= max_errors + index1);  // Delete remaining
    }

    if (s1[index1] == s2[index2]) {
        return accepts(s1, s2, max_errors, index1 + 1, index2 + 1, cur_errors);  // Correct
    }
    return accepts(s1, s2, max_errors, index1, index2 + 1, cur_errors + 1)         // Insert
           || accepts(s1, s2, max_errors, index1 + 1, index2 + 1, cur_errors + 1)  // Change
           || accepts(s1, s2, max_errors, index1 + 1, index2, cur_errors + 1);     // Delete
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int words;
    std::cin >> words;
    std::vector<std::string> dictionary(words);
    for (unsigned int i = 0; i < words; i++) {
        std::cin >> dictionary[i];
    }

    unsigned int max_distance, queries;
    std::cin >> max_distance >> queries;
    for (unsigned int i = 0; i < queries; ++i) {
        std::string query;
        std::cin >> query;
        unsigned int answer = 0;
        for (const std::string& word : dictionary) {
            answer += accepts(word, query, max_distance, 0, 0, 0);
        }
        std::cout << answer << '\n';
    }
}