#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <string>
#include <vector>

std::vector<unsigned int> naive_search(const std::string & needle, const std::string & haystack) {
    std::vector<unsigned int> ans;
    bool flag = true;
    for (unsigned int i = 0; i <= (haystack.length() - needle.length()); ++i) {
        for (unsigned int j = 0; j < needle.length(); ++j) {
            if (haystack[i + j] != needle[j]) {
                flag = false;
                break;
            }
        }

        if (flag) {
            ans.push_back(i);
        }

        flag = true;
    }

    return ans;
}

#endif
