#include "stress_test.hpp"

#include <algorithm>
#include <chrono>
#include <random>

template <typename T>
std::vector<T> test::StressTest<T>::generate_vector_random(T size_vec,
                                                           T max_val) {
  std::vector<long long> result;
  result.reserve(size_vec);

  // 1. Seed the engine using the clock (high precision)
  uint64_t seed = std::chrono::steady_clock::now().time_since_epoch().count();
  std::mt19937_64 rng(seed);

  // 2. Distribution (min, max)
  std::uniform_int_distribution<long long> dist(0, max_val);

  for (int i = 0; i < size_vec; ++i) {
    result.push_back(dist(rng));
  }

  return result;
}
