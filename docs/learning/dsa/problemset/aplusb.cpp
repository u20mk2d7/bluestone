// #include <bits/stdc++.h>
#include <cstdio>  // Use specific headers for smaller binary size
#include <iostream>

// Custom fast integer reader
inline int readInt() {
  int x = 0;
  char c = getchar_unlocked();
  // Skip non-digits (like spaces or newlines)
  while (c < '0' || c > '9') c = getchar_unlocked();

  // Build the number bit by bit
  while (c >= '0' && c <= '9') {
    x = (x << 3) + (x << 1) + (c - '0');  // x * 10 using bitwise shifts
    c = getchar_unlocked();
  }
  return x;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);

  int a = readInt();
  int b = readInt();
  printf("%d\n",
         a + b);  // printf is generally faster than cout for simple types
  return 0;
}
