#include <cassert>
#include <cstdlib>
#include <iostream>

/*
 * constraints: 1 <= a,b <= 2 * 10^9
 */

int gcd_naive(int a, int b) {
  int current_gcd = 1;
  for (int d = 2; d <= a && d <= b; d++) {
    if (a % d == 0 && b % d == 0) {
      if (d > current_gcd) {
        current_gcd = d;
      }
    }
  }
  return current_gcd;
}

/* https://en.wikipedia.org/wiki/Euclidean_division */
int gcd_euclidean_remainder(long a, long b) {
  if (b == 0) return a;

  int temp = 0;
  while (b != 0) {
    temp = b;
    b = a % b;
    a = temp;
  }
  return a;
}

/* https://en.wikipedia.org/wiki/Binary_GCD_algorithm */
int gcd_binary_steins(long a, long b) {
  // 1. handle base cases
  // gcd(0, b) = b; gcd(a, 0) = a; gcd(0, 0) = 0
  if (a == 0) return b;
  if (b == 0) return a;
  if (a == b) return a;

  // 2. find the greatest power of 2 that divides both a and b
  int k = 0;
  while (((a | b) & 1) == 0) {
    a >>= 1;  // Divide by 2 right shift
    b >>= 1;  // Divide by 2 right shift
    ++k;      // count of common factors of 2
  }

  // if a is even, divide it by 2 until to odd number.
  while ((a & 1) == 0) {
    a >>= 1;
  }

  do {
    // if b is even number, divide it b 2 until to odd number
    while ((b & 1) == 0) {
      b >>= 1;
    }

    // gcd(u, v) = gcd(u, v-u) if u, v is odd & u <= v
    if (a > b) {
      std::swap(a, b);
    }

    b = b - a;

  } while (b != 0);

  return a << k;  // Multiply 'a' by 2^k (left shift bitwise operator)
}

/* https://en.wikipedia.org/wiki/Lehmer%27s_GCD_algorithm */
int gcd_lehmers(long a, long b) {}

void stressTest() {
  while (true) {
    long a = std::rand() % 1000;
    long b = std::rand() % 100;
    std::cout << a << " " << b << std::endl;
    assert(gcd_euclidean_remainder(a, b) == gcd_binary_steins(a, b));
  }
}

int main() {
  // stressTest();

  int a, b;
  std::cin >> a >> b;
  std::cout << gcd_binary_steins(a, b) << std::endl;

  return 0;
}
