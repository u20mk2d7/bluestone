// According to the C++23 standard, what is the output of this program?

#include <iostream>

int main() {
  int i = 42;
  int j = 1;
  std::cout << i / --j;
}
