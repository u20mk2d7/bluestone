// https://cses.fi/problemset/task/1070

#include <iostream>

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int n;
  std::cin >> n;  // Reads the 5
  for (int i = 0; i < n; i++) {
    int val;
    std::cin >> val;  // Reads 10, then 20, then 30...
    std::cout << val << " ";
  }
  std::cout << ;
  return 0;
}