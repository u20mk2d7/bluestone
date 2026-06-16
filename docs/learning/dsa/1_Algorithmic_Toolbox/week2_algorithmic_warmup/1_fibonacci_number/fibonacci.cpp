#include <cassert>
#include <iostream>
#include <vector>

long long fib(int n) {
  if (n <= 1) return n;

  return (fib(n - 1) + fib(n - 2));
}

// Binet's floating-point precision loss
// https://en.wikipedia.org/wiki/Fibonacci_sequence#Binet's_formula
long long fib_binet(int n) {
  double phi = (1.0 + std::sqrt(5.0)) / 2.0;
  double psi = (1.0 - std::sqrt(5.0)) / 2.0;
  long long Fn = (std::pow(phi, n) - std::pow(psi, n)) / (phi - psi);
  return Fn;
}

long long fib_arr(long long n) {
  // base case
  if (n < 2) {
    return n;
  }
  std::vector<long long> F(n + 1);
  F[0] = 0;
  F[1] = 1;

  for (int i = 2; i <= n; ++i) {
    F[i] = F[i - 1] + F[i - 2];
  }
  return F[n];
}

void test_solution() {
  assert(fib_arr(3) == 2);
  assert(fib_arr(10) == 55);
  for (int n = 0; n < 20; ++n) {
    assert(fib_arr(n) == fib(n));
  }
}

int main() {
  int n = 0;
  std::cin >> n;

  std::cout << fib_arr(n) << '\n';
  // test_solution();
  return 0;
}
