#include <ios>
#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n, w; std::cin >> n >> w;
    std::vector <unsigned int> weights(n);
    for (unsigned int i = 0; i < n; ++i) std::cin >> weights[i];

    std::vector < std::vector<unsigned int> > dp(n + 1, std::vector<unsigned int>(w + 1, 0));
    for (unsigned int i = 1; i <= n; ++i)
    {
        unsigned int weight = weights[i - 1];
        std::vector<unsigned int>& curDp = dp[i];
        std::vector<unsigned int>& prevDp = dp[i - 1];
        for (unsigned int j = 1; j <= w; ++j)
        {
            if (j >= weight) curDp[j] = std::max(prevDp[j], prevDp[j - weight] + weight);
            else curDp[j] = prevDp[j];
        }
    }

    std::cout << dp[n][w] << "\n";
    std::vector <unsigned int> ans;
    std::vector <unsigned int>::reverse_iterator weightIter = weights.rbegin();
    for (std::vector < std::vector<unsigned int> >::reverse_iterator now = dp.rbegin(); now != dp.rend() - 1; ++now, ++weightIter)
    {
        if ((*now)[w] != (*(now + 1))[w])
        {
            ans.push_back(*weightIter);
            w -= *weightIter;
        }
    }
    std::sort(ans.begin(), ans.end());
    std::cout << ans.size() << "\n";
    for (unsigned int i = 0; i < ans.size(); ++i) std::cout << ans[i] << " ";
}