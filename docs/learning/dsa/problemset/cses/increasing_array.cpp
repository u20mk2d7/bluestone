// https://cses.fi/problemset/task/1094

#include <iostream>
#include <istream>
#include <vector>

int main() {
  // Speed up I/O for 10^6 characters
  std::ios::sync_with_stdio(false);
  std::cin.tie(NULL);

  long long n, current, previous, moves = 0;
  std::cin >> n >> previous;  // read the first number into previous

  if (n < 2) {
    std::cout << "0" << std::endl;
    return 0;
  }

  for (int i = 1; i < n; ++i) {
    std::cin >> current;

    if (previous > current) {
      moves += previous - current;
    } else {
      previous = current;
    }
  }
  std::cout << moves << std::endl;

  return 0;
}
