#include <ios>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <tuple>
#define Interval std::tuple<double, double, double>

bool operator<(const Interval& first, const Interval& second) {
    return std::get<1>(first) < std::get<1>(second);
}
bool operator<(const double first, const Interval& second) {
    return first < std::get<1>(second);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n;
    std::cin >> n;
    std::vector<Interval> intervals(n);
    for (unsigned int i = 0; i < n; ++i) {
        std::cin >> std::get<0>(intervals[i]) >> std::get<1>(intervals[i]) >>
            std::get<2>(intervals[i]);
    }
    std::sort(intervals.begin(), intervals.end(),
              static_cast<bool (*)(const Interval&, const Interval&)>(operator<));

    std::vector<double> dp(n + 1);
    dp[0] = 0;
    std::vector<double>::iterator dp_cur = dp.begin() + 1;
    for (std::vector<Interval>::iterator cur = intervals.begin(); cur != intervals.end();
         ++cur, ++dp_cur) {
        unsigned int last_taken_index =
            std::upper_bound(intervals.begin(), cur, std::get<0>(*cur),
                             static_cast<bool (*)(const double, const Interval&)>(operator<)) -
            intervals.begin();
        *dp_cur = std::max(dp[last_taken_index] + std::get<2>(*cur), *(dp_cur - 1));
    }
    std::cout << std::fixed << std::setprecision(5) << dp[n];
}