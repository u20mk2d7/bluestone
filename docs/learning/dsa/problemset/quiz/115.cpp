// According to the C++23 standard, what is the output of this program?

#include <iostream>

void f(int) { std::cout << "i"; }
void f(double) { std::cout << "d"; }
void f(float) { std::cout << "f"; }

int main() { f(1.0); }