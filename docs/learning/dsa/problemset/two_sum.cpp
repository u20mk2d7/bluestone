// https://leetcode.com/problems/two-sum/

#include <iostream>
#include <unordered_map>
#include <vector>

class TwoSum {
public:
  std::vector<int> twoSum_ver01(std::vector<int> &nums, int target) {
    std::vector<int> result;
    for (int i = 0; i < nums.size(); ++i) {
      for (int j = i + 1; j < nums.size(); ++j) {
        if (target == (nums[i] + nums[j])) {
          result.push_back(i);
          result.push_back(j);
        }
      }
    }
    return result;
  }

  // Fix one the numbers x, scan the entire array to find number y which is
  // (target - x)
  std::vector<int> twoSum_ver02(std::vector<int> &nums, int target) {
    std::unordered_map<int, int> seen;

    int comp = 0;
    for (int i = 0; i < nums.size(); ++i) {
      comp = target - nums[i];

      std::unordered_map<int, int>::iterator isFind = seen.find(comp);
      if (isFind != seen.end()) { // begin is first room of hallway,
        // and end() is exit door of hallway
        // .contains() c++20
        return {seen[comp], i};
      }

      seen[comp] = i; // compare with seen[nums[i]] = i
    }

    return {};
  }
};

int main() { return 0; }
