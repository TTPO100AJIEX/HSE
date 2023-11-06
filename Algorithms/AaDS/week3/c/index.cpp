#include <iostream>
#include <stack>
#include <string>

int main(void)
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::stack<char> opened;
    std::string data;
    std::cin >> data;
    for (char symbol : data)
    {
        if (symbol == '(' || symbol == '[' || symbol == '{') opened.push(symbol);
        else
        {
            char top = opened.top();
            if ((symbol == ')' && top == '(') || (symbol == ']' && top == '[') || (symbol == '}' && top == '{')) opened.pop();
            else
            {
                std::cout << "NO";
                return 0;
            }
        }
    }
    if (opened.empty()) std::cout << "YES";
    else std::cout << "NO";
    return 0;
}