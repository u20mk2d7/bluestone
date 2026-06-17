// https://leetcode.com/problems/merge-std::strings-alternately

#include <iostream>
#include <string>

class Solution {
public:
  std::string mergeAlternately_ver01(std::string word1, std::string word2);
  std::string mergeAlternately_ver02(std::string word1, std::string word2);
};

std::string Solution::mergeAlternately_ver01(std::string word1,
                                             std::string word2) {
  int size_of_word1 = word1.length();
  int size_of_word2 = word2.length();
  std::string mergestring;

  if (size_of_word1 >= size_of_word2) {
    for (int i = 0; i < size_of_word2;) {
      mergestring += word1[i];
      mergestring += word2[++i];
    }

    mergestring = word1.substr(size_of_word1 - size_of_word2, size_of_word1);
  } else {
    for (int i = 0; i < size_of_word1;) {
      mergestring += word2[i];
      mergestring += word1[i];
    }

    mergestring = word2.substr(size_of_word2 - size_of_word1, size_of_word2);
  }

  return mergestring;
}

/*
  Hint: Use two pointers, one pointer for each std::string.
  Alternately choose the character from each pointer, and move the
   pointer upwards.
 */
std::string Solution::mergeAlternately_ver02(std::string word1,
                                             std::string word2) {}

int main() {
  Solution solution;
  std::cout << solution.mergeAlternately_ver01("abc", "pqr") << std::endl;
  std::cout << solution.mergeAlternately_ver02("abc", "pqr") << std::endl;
}
