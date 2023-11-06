#include <ios>
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n, w;
    std::cin >> n >> w;
    std::vector<unsigned int> weights(n);
    for (unsigned int i = 0; i < n; ++i) {
        std::cin >> weights[i];
    }

    std::vector<std::vector<unsigned int> > dp(n + 1, std::vector<unsigned int>(w + 1));
    for (unsigned int i = 1; i <= n; ++i) {
        dp[i][0] = 0;
    }
    for (unsigned int j = 0; j <= w; ++j) {
        dp[0][j] = 0;
    }
    for (unsigned int i = 1; i <= n; ++i) {
        unsigned int weight = weights[i - 1];
        std::vector<unsigned int>& cur_dp = dp[i];
        std::vector<unsigned int>& prev_dp = dp[i - 1];
        for (unsigned int j = 1; j <= w; ++j) {
            if (j >= weight) {
                cur_dp[j] = std::max(prev_dp[j], prev_dp[j - weight] + weight);
            } else {
                cur_dp[j] = prev_dp[j];
            }
        }
    }

    std::cout << dp[n][w] << "\n";
    std::vector<unsigned int> ans;
    std::vector<unsigned int>::reverse_iterator weight_iter = weights.rbegin();
    for (std::vector<std::vector<unsigned int> >::reverse_iterator now = dp.rbegin();
         now != dp.rend() - 1; ++now, ++weight_iter) {
        if ((*now)[w] != (*(now + 1))[w]) {
            ans.push_back(*weight_iter);
            w -= *weight_iter;
        }
    }
    std::sort(ans.begin(), ans.end());
    std::cout << ans.size() << "\n";
    for (unsigned int i = 0; i < ans.size(); ++i) {
        std::cout << ans[i] << " ";
    }
}