#include <ios>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

struct Interval
{
    double left, right, weight;
    bool operator< (const Interval& other) { return this->right < other.right; }
    friend bool operator< (const double first, const Interval& other) { return other.right > first; }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n; std::cin >> n;
    std::vector <Interval> intervals(n);
    for (unsigned int i = 0; i < n; ++i) std::cin >> intervals[i].left >> intervals[i].right >> intervals[i].weight;
    std::sort(intervals.begin(), intervals.end());
    
    std::vector <double> dp(n + 1); dp[0] = 0;
    std::vector <double>::iterator dpCur = dp.begin() + 1;
    for (std::vector <Interval>::iterator cur = intervals.begin(); cur != intervals.end(); ++cur, ++dpCur)
    {
        unsigned int lastTakenIndex = std::upper_bound(intervals.begin(), cur, cur->left) - intervals.begin();
        *dpCur = std::max(dp[lastTakenIndex] + cur->weight, *(dpCur - 1));
    }
    std::cout << std::fixed << std::setprecision(5) << dp[n];
}