// According to the C++23 standard, what is the output of this program?

#include <iostream>
#include <vector>

int main() {
  std::vector<int> v1(1, 2);
  std::vector<int> v2{1, 2};
  std::cout << v1.size() << v2.size();
}