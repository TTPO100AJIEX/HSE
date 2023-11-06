#include <iostream>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    unsigned int n, sum = 0, max_divided = 0;
    std::cin >> n;
    for (unsigned int i = 0; i < n; i++)
    {
        int num_s; std::cin >> num_s;
        if (num_s == 0) continue;
        unsigned int number = abs(num_s), divided = 0;
        sum++;
        while (number > 1)
        {
            sum += (number & 1);
            number >>= 1;
            divided++;
        }
        if (divided > max_divided) max_divided = divided;
    }
    std::cout << max_divided + sum;

    return 0;
}