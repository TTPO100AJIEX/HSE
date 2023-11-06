#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n, m;
    std::cin >> n >> m;
    std::vector<unsigned int> cur(m + 1, 0);
    unsigned int ans = 0;
    for (unsigned int i = 0; i < n; ++i)
    {
        for (unsigned int j = 1, prev = 0; j <= m; ++j)
        {
            bool number;
            std::cin >> number;
            if (number)
            {
                prev = std::min(prev, std::min(cur[j - 1], cur[j])) + 1;
                ans = std::max(ans, prev);
                std::swap(cur[j], prev);
            }
            else
            {
                prev = cur[j];
                cur[j] = 0;
            }
        }
    }
    std::cout << ans;
}