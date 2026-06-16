#ifndef STRESS_TEST
#define STRESS_TEST
#include <iostream>
#include <string>
#include <vector>

namespace test {
template <typename T> class StressTest {
private:
public:
  static std::vector<T> generate_vector_random(T size, T max_range);
}; // class StressTest

} // namespace test
#endif // STRESS_TEST
