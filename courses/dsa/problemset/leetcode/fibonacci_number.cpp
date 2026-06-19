// https://leetcode.com/problems/fibonacci-number

#include <iostream>
#include <random>

class Solution {
 public:
  int fib(int n) {
    if (n <= 1) return n;
    // 0 1 1 2 3 5
    //
    int prev = 0;
    int curr = 1;
    for (int i = 2; i <= n; ++i) {
      int temp = prev + curr;
      prev = curr;
      curr = temp;
    }
    return curr;
  }
};

void stress_test(Solution sol) {
  std::random_device rd;   // Obtain a random seed from the hardware
  std::mt19937 gen(rd());  // Standard Mersenne Twister engine
  std::uniform_int_distribution<int> distrib(0, 35);
  while (true) {
    int n = distrib(gen);
    std::cout << sol.fib(n) << '\n';
  }
}

int main() {
  Solution solution;
  stress_test(solution);
  return 0;
}
