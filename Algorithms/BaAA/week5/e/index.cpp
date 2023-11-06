#include <ios>
#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n, l; std::cin >> l >> n; n += 2;
    std::vector <unsigned int> points(n); points[0] = 0; points[n - 1] = l;
    std::vector < std::vector <unsigned int> > dp(n, std::vector <unsigned int>(n));
    for (unsigned int i = 0; i < n - 2; ++i) { dp[i] = std::vector <unsigned int>(n - i - 1); dp[i][0] = 0; std::cin >> points[i + 1]; }
    
    for (int i = n - 2; i >= 0; --i)
    {
        for (unsigned int j = 0; j < n - i - 2; ++j)
        {
            unsigned int bestSplit = dp[i][0] + dp[i + 1][j];
            for (unsigned int splitpoint = 1; splitpoint <= j; ++splitpoint) bestSplit = std::min(dp[i][splitpoint] + dp[splitpoint + i + 1][j - splitpoint], bestSplit);
            dp[i][j + 1] = bestSplit + points[j + i + 2] - points[i];
        }
    }
    std::cout << dp[0][n - 2];
}