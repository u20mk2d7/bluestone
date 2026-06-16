// https://leetcode.com/problems/find-the-difference

#include <iostream>
#include <string>

class Solution {
public:
  char findTheDifference_ver01(std::string s, std::string t);
  char findTheDifference_ver02(std::string s, std::string t);
};

char Solution::findTheDifference_ver01(std::string s, std::string t) {
  int a = 0;
  int b = 0;

  for (char c : s) {
    std::cout << "a: " << a << " + c: '" << c << "' = " << (int)c << std::endl;
    a += c;
  }
  std::cout << "Sum of std::string s = " << a << std::endl;

  for (char d : t) {
    std::cout << "b: " << b << " + d: '" << d << "' = " << (int)d << std::endl;
    b += d;
  }

  std::cout << "Sum of std::string t = " << b << std::endl;

  return b - a;
}

/*
                std::string s <= std::string t
                => diff = tSum - sSum
*/
char Solution::findTheDifference_ver02(std::string s, std::string t) {
  for (int i = 0; i < s.size(); ++i) {
    t[i + 1] += t[i] - s[i]; // Passing the diff: (t[i] - s[i]) to t[i + 1]
  }

  return t[t.size() - 1]; // the diff will be carried over to the last element
                          // eventually
}

int main() {
  Solution solution;
  std::cout << solution.findTheDifference_ver02("abc", "dabc") << std::endl;

  return 0;
}
