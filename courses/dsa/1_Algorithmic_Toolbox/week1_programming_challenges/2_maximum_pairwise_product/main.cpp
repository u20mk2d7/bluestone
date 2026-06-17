#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
#include <unordered_map>
#include <vector>

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

  return (long long)numbers[max1] * numbers[max2];
}

int main() {
  std::vector<long long> numbers;
  unsigned int size;
  std::cin >> size;
  numbers.resize(size);
  for (int i = 0; i < numbers.size(); ++i) {
    std::cin >> numbers[i];
  }

  std::cout << MaxPairwiseProduct_rev02(numbers) << std::endl;

  return 0;
}
