// https://cses.fi/problemset/task/1083

#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main() {
  long long n = 0LL;
  std::cin >> n;
  int temp;
  long long actualSum = 0;

  // We expect n-1 numbers because one is missing
  for (int i = 0; i < n - 1; ++i) {
    // This skips spaces AND newlines automatically!
    std::cin >> temp;
    actualSum += temp;
  }

  long long exceptedSum = (n * (n + 1)) / 2;
  std::cout << (exceptedSum - actualSum);
  return 0;
}
