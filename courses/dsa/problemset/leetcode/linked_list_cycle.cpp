// https://leetcode.com/problems/linked-list-cycle

#include <iostream>
struct ListNode {
  ListNode* next;
  int val;
};

// Floyd's cycle-finding algorithm
class Solution {
 public:
  bool hasCycle_ver01(ListNode* head);
  bool hasCycle_ver02(ListNode* head);
};

bool Solution::hasCycle_ver01(ListNode* head) {
  if (head == nullptr || head->next == nullptr) {
    return false;  // No cycle if list is empty or has only one node
  }

  ListNode* slow = head;
  ListNode* fast = head;

  while (fast != nullptr && fast->next != nullptr) {
    slow = slow->next;        // Move slow pointer one step
    fast = fast->next->next;  // Move fast pointer two steps

    if (slow == fast) {
      return true;  // Cycle detected
    }
  }

  return false;
}

bool Solution::hasCycle_ver02(ListNode* head) {
  if (head == nullptr || head->next == nullptr) return false;

  ListNode* current = head;
  ListNode* after = head->next;
  while (after) {
    if (after == nullptr || after->next == nullptr) return false;
    if (after == current) return true;
    after = after->next->next;
    current = current->next;
  }

  return false;
}
