// https://cses.fi/problemset/task/1069

#include <assert.h>

#include <iostream>
#include <string>

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);

  std::string dna;
  if (!(std::cin >> dna)) return 0;
  size_t n = dna.length();
  size_t count = 1, max_count = 1;

  if (n == 0) {
    std::cout << 0 << std::endl;
    return 0;
  }

  for (size_t i = 1; i < n; ++i) {
    if (dna[i] == dna[i - 1]) {
      ++count;
    } else if (dna[i] != dna[i - 1]) {
      max_count = std::max(max_count, count);
      count = 1;
    }
  }

  max_count = std::max(max_count, count);
  std::cout << max_count << std::endl;
  return 0;
}
