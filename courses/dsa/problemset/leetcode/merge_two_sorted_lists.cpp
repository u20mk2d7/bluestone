// https://leetcode.com/problems/merge-two-sorted-lists

#include <iostream>

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */

class Solution {
 public:
  ListNode* mergeTwoLists_ver01(ListNode* list1, ListNode* list2) {
    if (list1 == nullptr) return list2;
    if (list2 == nullptr) return list1;

    ListNode dummy(0);
    ListNode* current = dummy;

    while (list1 && list2) {
      if (list1->val <= list2->val) {
        current->next = list1;
        list1 = list1->next;
      } else {
        current->next = list2;
        list2 = list2->next;
      }

      current = current->next;
    }

    if (list1) {
      current->next = list1;
    }

    if (list2) {
      current->next = list2;
    }

    return dummy->next;
  }

  ListNode* mergeTwoLists_ver02(ListNode* list1, ListNode* list2) {
    if (list1 == nullptr) return list2;
    if (list2 == nullptr) return list1;

    if (list1->val < list2->val) {
      list1->next = mergeTwoLists_ver02(list1->next, list2);
      return list1;
    } else {
      list2->next = mergeTwoLists_ver02(list1, list2->next);
      return list2;
    }
  }
};
