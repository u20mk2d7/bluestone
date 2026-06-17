#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

int get_fib_last_digit_navie(int n) {
  if (n < 2) {
    return n;
  }

  std::vector<int> F(n + 1);
  F[0] = 0;
  F[1] = 1;
  for (int i = 2; i <= n; ++i) {
    F[i] = (F[i - 1] + F[i - 2]) % 10;
  }
  return F[n];
}

// https://en.wikipedia.org/wiki/Pisano_period
int get_fib_last_digit_pisano_period(int n) {
  // Base cases for F(0) = 0 and F(1) = 1
  if (n < 2) {
    return n;
  }

  // Apply the Pisano Period: F(n) % 10 is the same as F(n % 60) % 10.
  // This reduces 'n' to a smaller equivalent index within the repeating cycle
  n = n % 60;

  // Handle the case where n % 60 results in 0 (e.g., for F_60, F_120, etc.).
  // F_0 is 0, and since F_60 ends in 0, this mapping is correct.
  // The loop below correctly handles n=0 and n=1 as base cases too.
  if (n == 0) {  // F_0 is 0. Also, F_60 % 10 is 0, F_120 % 10 is 0, etc.
    return 0;
  }

  // Initialize previous and current Fibonacci numbers (last digits)
  int tmp = 0;  // Temporary variable for calculation
  int prev = 0;
  int curr = 1;

  // Iterate from the 2nd term up to the (n % 60)th term
  for (int i = 2; i <= n; ++i) {
    // Calculate the next Fibonacci last digit
    // Only store the last digit by taking modulo 10 at each step
    tmp = (prev + curr) % 10;
    prev = curr;
    curr = tmp;
  }

  return curr;  // 'curr' now holds the last digit of the nth Fibonacci number
}

void stress_test() {
  for (int n = 0; n < 1000000; ++n) {
    assert(get_fib_last_digit_pisano_period(n) == get_fib_last_digit_navie(n));
  }
}

int main() {
  // stress_test();

  int n;
  std::cin >> n;
  int c = get_fib_last_digit_pisano_period(n);
  std::cout << c << '\n';

  return 0;
}
