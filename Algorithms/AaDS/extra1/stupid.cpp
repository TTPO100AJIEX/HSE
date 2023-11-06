#include <iostream>
#include <vector>

int main(void)
{
    //std::ios::sync_with_stdio(false);
    //std::cin.tie(nullptr);

    char type;
    int mod;
    std::cin >> type;
    if (type == '+') std::cin >> mod;
    int n;
    std::cin >> n;
    std::vector<int> A(n);
    for (int i = 0; i < n; i++) std::cin >> A[i];
    int q;
    std::cin >> q;
    for (int i = 0; i < q; i++)
    {
        std::string command;
        std::cin >> command;
        if (command == "insert")
        {
            int pos, x;
            std::cin >> pos >> x;
            pos--;
            A.insert(A.begin() + pos, x);
            continue;
        }
        if (command == "erase")
        {
            int pos;
            std::cin >> pos;
            pos--;
            A.erase(A.begin() + pos);
            continue;
        }
        if (command == "change")
        {
            int pos, x;
            std::cin >> pos >> x;
            pos--;
            A[pos] = x;
            continue;
        }
        if (command == "at")
        {
            int pos;
            std::cin >> pos;
            pos--;
            std::cout << A[pos] << "\n";
            continue;
        }
        if (command == "compute")
        {
            int l, r;
            std::cin >> l >> r;
            l--; r--;
            int ans = A[l++];
            if (type == '+')
            {
                ans %= mod;
                if (ans < 0) ans += mod;
            }
            while (l <= r)
            {
                if (type == '+')
                {
                    ans = (ans + A[l]) % mod;
                    if (ans < 0) ans += mod;
                }
                else ans ^= A[l];
                l++;
            }
            std::cout << ans << "\n";
        }
        if (command == "size")
        {
            std::cout << A.size() << "\n";
            continue;
        }
    }

    return 0;
}