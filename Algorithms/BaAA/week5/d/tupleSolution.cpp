#include <ios>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <tuple>
#define Interval std::tuple <double, double, double>
// bool operator< (const Interval& first, const Interval& second) { return std::get<1>(first) < std::get<1>(second); }
//bool operator< (const double first, const Interval& second) { return first < std::get<1>(second); }

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n; std::cin >> n;
    std::vector <Interval> intervals(n);
    for (unsigned int i = 0; i < n; ++i) std::cin >> std::get<0>(intervals[i]) >> std::get<1>(intervals[i]) >> std::get<2>(intervals[i]);
    std::sort(intervals.begin(), intervals.end(), [](const Interval& first, const Interval& second) { return std::get<1>(first) < std::get<1>(second); });
    
    std::vector <double> dp(n + 1); dp[0] = 0;
    std::vector <double>::iterator dpCur = dp.begin() + 1;
    for (std::vector <Interval>::iterator cur = intervals.begin(); cur != intervals.end(); ++cur, ++dpCur)
    {
        unsigned int lastTakenIndex = std::upper_bound(intervals.begin(), cur, std::get<0>(*cur), [](const double first, const Interval& second) { return first < std::get<1>(second); }) - intervals.begin();
        *dpCur = std::max(dp[lastTakenIndex] + std::get<2>(*cur), *(dpCur - 1));
    }
    std::cout << std::fixed << std::setprecision(5) << dp[n];
}