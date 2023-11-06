#include <ios>
#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>

#include <chrono>

int main() {
    auto start = std::chrono::steady_clock::now();

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n, m;
    n = m = 2000;
    std::vector<std::string> field(n);
    for (unsigned int i = 0; i < n; ++i) {
        for (unsigned int j = 0; j < m; ++j) {
            field[i] += 'a';
        }
    }
    /*unsigned int n, m; std::cin >> n >> m;
    std::vector<std::string> field(n);
    for (unsigned int i = 0; i < n; ++i) std::cin >> field[i];*/

    uint64_t ans = n * m;
    std::vector<uint64_t> prev_dp(m, 1), cur_dp(m);
    --m;
    cur_dp[0] = cur_dp[m] = 1;
    for (unsigned int i = 1; i < n; ++i) {
        for (unsigned int j = 1; j < m; ++j) {
            char cur_symbol = field[i][j];
            if (cur_symbol != field[i - 1][j - 1] || cur_symbol != field[i - 1][j] ||
                cur_symbol != field[i - 1][j + 1]) {
                cur_dp[j] = 1;
                continue;
            }
            unsigned int best = std::min(prev_dp[j - 1], std::min(prev_dp[j], prev_dp[j + 1]));
            cur_dp[j] = best + ((i > best && cur_symbol == field[i - best - 1][j]) << 1);
            ans += (cur_dp[j] >> 1);
        }
        std::swap(prev_dp, cur_dp);
    }

    std::cout << ans;

    auto end = std::chrono::steady_clock::now();
    std::cout << "\nTime spent: "
              << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / 1e9
              << "s";
}
