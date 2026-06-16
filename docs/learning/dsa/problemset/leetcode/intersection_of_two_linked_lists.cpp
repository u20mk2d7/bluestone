#include <iostream>
#include <unordered_map>

struct ListNode {
  ListNode* next;
  int val;
};

class Solution {
 public:
  ListNode* getIntersectionNode_ver01(ListNode* headA, ListNode* headB);
  ListNode* getIntersectionNode_ver02(ListNode* headA, ListNode* headB);
};

ListNode* Solution::getIntersectionNode_ver01(ListNode* headA,
                                              ListNode* headB) {
  if (headA == nullptr || headB == nullptr) return nullptr;

  ListNode* temp1 = headA;
  ListNode* temp2 = headB;

  while (temp1 != temp2) {
    if (temp1 == nullptr) {
      temp1 = headB;
    } else {
      temp1 = temp1->next;
    }

    if (temp2 == nullptr) {
      temp2 = headA;
    } else {
      temp2 = temp2->next;
    }

    if (temp1 == temp2) {
      return temp1;
    }
    /*
    temp1 = (temp1 == nullptr) ? headB : temp1->next;
    temp2 = (temp2 == nullptr) ? headA : temp2->next;
    */

  }  // end while

  return temp1;
}

ListNode* Solution::getIntersectionNode_ver02(ListNode* headA,
                                              ListNode* headB) {
  std::unordered_map<ListNode*, int> mpp;
  for (ListNode* p = headA; p != NULL; p = p->next) {
    mpp[p] = p->val;
  }

  for (auto p = headB; p != NULL; p = p->next) {
    if (mpp.find(p) != mpp.end()) return p;
  }
  return NULL;
}
