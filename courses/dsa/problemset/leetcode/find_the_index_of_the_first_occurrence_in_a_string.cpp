// https://leetcode.com/problems/find-the-index-of-the-first-occurrence-in-a-string

#include <iostream>
#include <string>

class Solution {
public:
  int strStr_ver01(std::string haystack, std::string needle);

  // https://www.geeksforgeeks.org/kmp-algorithm-for-pattern-searching/
  int strStr_ver02(std::string haystack, std::string needle);
};

int Solution::strStr_ver01(std::string haystack, std::string needle) {
  return haystack.find(needle);
}

int Solution::strStr_ver02(std::string haystack, std::string needle) {}

int main() {
  Solution solution;
  std::string haystack =
      "We think in generalities, but we live in details."; // (quoting Alfred
                                                           // N.
  // Whitehead)
  std::string needle = "in";
  size_t pos = haystack.find(needle);

  solution.strStr_ver01(haystack, needle);

  return 0;
}
