#include <string_view>
#include <set>
#include <vector>

std::set<unsigned int> search(std::string_view text, std::string_view pattern)
{
    std::set<unsigned int> answer;
    if (text.size() < pattern.size()) return answer;
    
    // Split pattern into parts by substitution symbol
    std::vector<std::string_view> parts;
    unsigned int partStart = 0;
    for (unsigned int i = 0; i < pattern.size(); ++i)
    {
        if (pattern[i] == '?')
        {
            parts.push_back(pattern.substr(partStart, i - partStart));
            partStart = i + 1;
        }
    }
    parts.push_back(pattern.substr(partStart, pattern.size() - partStart));

    // Look for parts separately and check if they follow each other
    bool isFirst = true;
    unsigned int matchedLength = 0;
    for (std::string_view part : parts)
    {
        if (part.size() == 0) { ++matchedLength; continue; } // If the part is empty, just match the substitution symbol

        // Compute blocks for the part
        std::vector <unsigned int> patternBlocks(part.size());
        patternBlocks[0] = 0;
        for (unsigned int i = 1; i < part.size(); ++i)
        {
            unsigned int prefixSize = patternBlocks[i - 1];
            while (prefixSize != 0 && part[i] != part[prefixSize]) prefixSize = patternBlocks[prefixSize - 1];
            patternBlocks[i] = prefixSize + (part[i] == part[prefixSize]);
        }

        std::set<unsigned int>::iterator answerIter = answer.begin(); // Iterator to delete answers that do not continue
        /*
            Change the limits of searching to increase performance and remove answers that are definitely incorrect:
            - If there is a match before "begin", it is definitely not correct (there is not enough elements before it to match the pattern)
            - If there is a match after "end", it is definitely not correct (there is not enough elements after it to match the pattern)
        */
        unsigned int begin = (answer.empty() ? 0 : *(answer.begin())) + matchedLength;
        unsigned int end = text.size() - pattern.size() + matchedLength + part.size();
        for (unsigned int i = begin, matched = 0; i < end; ++i)
        {
            while (matched != 0 && part[matched] != text[i]) matched = patternBlocks[matched - 1];
            if (part[matched] == text[i]) ++matched;

            if (matched == part.size())
            {
                matched = patternBlocks[matched - 1];
                unsigned int index = i - part.size() + 1 - matchedLength; // The beginning of the possible match
                if (isFirst)
                {
                    // If it is the first part, just save the answer
                    answer.insert(index);
                }
                else
                {
                    // Remove answers that deemed correct before but proved wrong now
                    while (answerIter != answer.end() && *answerIter < index) answer.erase(answerIter++);
                    if (answerIter != answer.end() && *answerIter == index) answerIter++; // The answer is correct
                }
            }
        }
        // Everything else is proved wrong
        while (answerIter != answer.end()) answer.erase(answerIter++);
        // Adjust the parameters
        matchedLength += part.size() + 1;
        isFirst = false;
    }
    if (isFirst)
    {
        // If no parts were empty, the pattern is just some subtituion symbols => every index is an answer if the pattern fits
        for (unsigned int i = 0; i <= text.size() - pattern.size(); ++i) answer.insert(i);
    }
    return answer;
}