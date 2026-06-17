// https://codeforces.com/problemset/problem/4/A

#include <iostream>

int main() {
  // Optimization for fast I/O
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);

  long long watermelon_kilos;
  std::cin >> watermelon_kilos;

  if (watermelon_kilos % 2 == 0 && watermelon_kilos > 2) {
    std::cout << "YES\n";
  } else {
    std::cout << "NO\n";
  }
  return 0;
}