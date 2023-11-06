#include <iostream>
#include <vector>
#include <algorithm>

int find_doubled_median(int* A, int* B, unsigned int size)
{
    if (size == 2) return(A[0] + A[1] + B[0] + B[1] - std::min(std::min(A[0], A[1]), std::min(B[0], B[1])) - std::max(std::max(A[0], A[1]), std::max(B[0], B[1])));
    int part_medians = (size - 1) / 2;
    if (A[part_medians] < B[part_medians]) return find_doubled_median(A + part_medians, B, size - part_medians);
    else return find_doubled_median(A, B + part_medians, size - part_medians);
}

int main(void)
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n, m;
    std::cin >> n >> m;
    std::vector< std::vector<int> > data(n, std::vector<int>(m));
    for (unsigned int i = 0; i < n; i++)
    {
        for (unsigned int j = 0; j < m; j++) std::cin >> data[i][j];
    }

    for (unsigned int i = 0; i < n; i++)
    {
        for (unsigned int j = i + 1; j < n; j++)
        {
            int median = find_doubled_median(data[i].data(), data[j].data(), m);
            if (median < 0) { std::cout << "-"; median = -median; }
            if (median % 2 == 1) std::cout << median / 2 << ".50000\n";
            else std::cout << median / 2 << ".00000\n";
        }
    }

    return 0;
}