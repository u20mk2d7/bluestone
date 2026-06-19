// https://leetcode.com/problems/remove-duplicates-from-sorted-list

#include <iostream>
#include <unordered_set>

class Solution {
 public:
  ListNode* deleteDuplicates_ver01(ListNode* head) {
    ListNode* temp = head;
    ListNode* current = head;
    // 1 -> 1 -> 21 -> 1 -> 1 -> 1 -> 1 -> 1 -> nullptr
    while (current != nullptr && current->next != nullptr) {
      if (current->val == current->next->val) {
        temp = current->next;
        current->next = current->next->next;
        delete temp;
      } else {
        current = current->next;
      }
    }

    return head;
  }
};
