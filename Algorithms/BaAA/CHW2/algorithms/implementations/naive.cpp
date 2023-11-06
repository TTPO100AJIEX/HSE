#include <string_view>
#include <set>

std::set<unsigned int> search(std::string_view text, std::string_view pattern)
{
    std::set<unsigned int> answer;
    if (text.size() < pattern.size()) return answer;
    for (unsigned int i = 0; i <= text.size() - pattern.size(); ++i)
    {
        bool matches = true;
        for (unsigned int j = 0; j < pattern.size(); ++j)
        {
            if (pattern[j] != text[i + j] && pattern[j] != '?')
            {
                matches = false;
                break;
            }
        }
        if (matches) answer.insert(i);
    }
    return answer;
}