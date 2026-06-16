// https://leetcode.com/problems/palindrome-number/

#include <iostream>
#include <vector>

class PalindromeNumber {
 public:
  bool isPalindrome_rev01(int x) {
    if (x < 0 || (x % 10 == 0 && x != 0)) return false;

    if (x > 0 && x < 10) return true;

    int reverse = 0, temp = x;
    while (temp > 0) {
      reverse = (reverse * 10) + (temp % 10);
      temp /= 10;
    }

    return (reverse == x);
  }

  // https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
  bool isPalindrome_rev02(int x) {
    std::vector<int> prime_nums;
    int prime_num = 2;
    int index = 0;

    for (int i = 2; i < x; ++i) {
    }

    return false;
  }
};

int main() {
  PalindromeNumber p;
  p.isPalindrome_rev02(120);
  return 0;
}
