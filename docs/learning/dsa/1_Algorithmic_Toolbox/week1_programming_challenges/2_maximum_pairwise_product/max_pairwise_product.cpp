#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
#include <unordered_map>
#include <vector>

long long MaxPairwiseProduct_rev01(std::vector<long long>& numbers) {
  long long result = 0;
  int size = numbers.size();
  int max1 = 0, max2 = 0;

  for (int i = 0; i < size; ++i) {
    for (int j = i + 1; j < size; ++j) {
      if ((numbers[i] * numbers[j]) > result) {
        result = numbers[i] * numbers[j];
        max1 = numbers[i];
        max2 = numbers[j];
      }
    }
  }

  std::cout << "rev01: " << max1 << " - " << max2 << "\n";
  return result;
}

long long MaxPairwiseProduct_rev02(std::vector<long long>& numbers) {
  int size = numbers.size();
  if (size < 2) return 0;

  int max1 = 0;
  for (int i = 0; i < size; ++i) {
    if (numbers[max1] < numbers[i]) {
      max1 = i;
    }
  }

  int max2 = 1;
  for (int i = 0; i < size; ++i) {
    if (max2 == max1) {
      --max2;
    }

    if ((i != max1) && (numbers[max2] < numbers[i])) {
      max2 = i;
    }
  }
  std::cout << "rev02: " << numbers[max1] << " - " << numbers[max2]
            << std::endl;

  return (long long)numbers[max1] * numbers[max2];
}

int main() {
  // Constraints 2 ≤ n ≤ 2 * 10^5; 0 ≤ a1,...,an ≤ 2 · 105
  // 1. Seed with high-precision clock
  // std::mt19937_64 rng(1234567890123ULL);
  std::mt19937_64 rng(
      std::chrono::steady_clock::now().time_since_epoch().count());

  // 2. Define our ranges (Size and Values)
  std::uniform_int_distribution<int> size_dist(0, 10000);
  std::uniform_int_distribution<int> value_dist(1, 10000LL);  // 1-1e9

  // 3. The Test Loop
  // Generate a random size for this specific test
  while (true) {
    // 3. The Test Loop
    // Generate a random size for this specific test
    std::vector<long long> test_data;
    int current_size = size_dist(rng);

    // Create and fill the vector
    test_data.reserve(current_size);  // Efficiency: pre-allocate memory

    for (int i = 0; i < current_size; ++i) {
      test_data.push_back(value_dist(rng));
    }

    // Print the test case
    std::cout << "Array: ";
    for (auto x : test_data) std::cout << x << " ";
    std::cout << "\n";

    long long a = MaxPairwiseProduct_rev01(test_data);
    long long b = MaxPairwiseProduct_rev02(test_data);
    std::cout << "\n";

    if (a != b) {
      std::cout << "[FAIL] ";
      std::cout << "rev01: " << a << " vs rev02: " << b << std::endl;
      break;
    }
  }

  return 0;
}
