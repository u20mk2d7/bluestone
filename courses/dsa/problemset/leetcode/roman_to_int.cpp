// https://leetcode.com/problems/roman-to-integer/

#include <iostream>
#include <string>
#include <unordered_map>

class Solution {
 public:
  int romanToInt(std::string s) {
    std::unordered_map<char, int> roman = {{'I', 1},   {'V', 5},   {'X', 10},
                                           {'L', 50},  {'C', 100}, {'D', 500},
                                           {'M', 1000}};

    int result = 0;
    for (size_t i = 0; i < s.length(); ++i) {
      if ((i + 1 < s.length()) && roman[s[i]] >= roman[s[i + 1]]) {
        result = result + roman[s[i]];
      } else if (roman[s[i]] < roman[s[i + 1]]) {
        result = result - roman[s[i]];
      }
    }
    std::cout << result;
    return result;
  }
};

int main() {
  Solution solution;

  std::string s;
  std::cin >> s;
  solution.romanToInt(s);
  return 0;
}
