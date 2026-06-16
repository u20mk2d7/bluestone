// According to the C++ 23 standard, what is the output of this program ?

#include <iostream>

class A {
public:
  inline virtual void f() { std::operator<<(std::cout, "A"); }

  // inline constexpr A() noexcept = default;
};

class B : public A {
private:
  inline virtual void f() { std::operator<<(std::cout, "B"); }

public:
  // inline constexpr B() noexcept = default;
};

void g(A &a) { a.f(); }

int main() {
  B b = B();
  g(static_cast<A &>(b));
  return 0;
}
