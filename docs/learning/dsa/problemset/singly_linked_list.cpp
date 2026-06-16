#include <iostream>

class LinkedList {
private:
  Node *head;
  Node *tail;
  int length;

public:
  LinkedList(int value) {
    Node *new_node = new Node(value);
    head = new_node;
    tail = new_node;
    length = 1;
  }

  ~LinkedList() {
    Node *current = head;
    while (current != nullptr) {
      Node *next_node = current->next;
      delete current;
      current = next_node;
    }
    head = nullptr;
  }

  void print_list() {
    Node *temp = head;

    while (temp != nullptr) {
      cout << temp->val << " -> ";
      temp = temp->next;
    }
    cout << "nullptr\n";
  }

  void append(int value) {
    Node *new_node = new Node(value);
    if (head == nullptr) {
      head = new_node;
      tail = new_node;
    }

    Node *current = head;
    while (current->next != nullptr) {
      current = current->next;
    }

    current->next = new_node; // tail->next = new_node;
    // tail = new_node;

    ++length;
  }

  void delete_last() {
    if (length == 0)
      return;
    if (length == 1) {
      head = nullptr;
      tail = nullptr;
    } else {
      Node *temp = head;
      Node *pre = head;
      while (temp->next) {
        pre = temp;
        temp = temp->next;
      }

      tail = pre;
      tail->next = nullptr;
      delete temp;
      delete pre; // (?) line tail = pre; Segmentation fault
    }

    --length;
  }

  void prepend(int value) {
    Node *new_node = new Node(value);
    if (length == 0) {
      head = new_node;
      tail = new_node;
    } else {
      new_node->next = head;
      head = new_node;
    }

    ++length;
  }

  void delete_first() {
    if (length == 0)
      return;
    Node *temp = head;
    if (length == 1) {
      head = nullptr;
      tail = nullptr;
    } else {
      head = head->next;
    }
    delete temp;
    length--;
  }

  Node *get(int index) {
    if (index < 0 || index >= length)
      return nullptr;
    Node *temp = head;
    for (int i = 0; i < index; ++i) {
      temp = temp->next;
    }
    return temp;
  }

  bool set(int index, int value) {
    Node *temp = get(index);
    if (temp) {
      temp->val = value;
      return true;
    }
    return false;
  }

  bool insert(int index, int value) {
    if (index < 0 || index > length)
      return false;

    if (index == 0) {
      prepend(value);
      return true;
    } else if (index == length) {
      append(value);
      return true;
    }

    Node *new_node = new Node(value);
    Node *temp = get(index - 1);
    new_node->next = temp->next;
    temp->next = new_node;
    ++length;

    return false;
  }

  Node *find_middle_node() {
    if (head == nullptr)
      return nullptr;
    if (head == tail)
      return head;

    Node *slow = head;
    Node *fast = head;
    while (fast != nullptr && fast->next != nullptr) {
      fast = fast->next->next;
      slow = slow->next;
    }

    return slow;
  }

  bool has_loop() {
    Node *slow = head;
    Node *fast = head;
    while (fast != nullptr && fast->next != nullptr) {
      slow = slow->next;
      fast = fast->next->next;
      if (slow == fast) {
        return true;
      }
    }
    return false;
  }

  Node *find_Kth_from_end(int k) {
    Node *slow = head;
    Node *fast = head;

    for (int i = 0; i < k; ++i) {
      if (fast == nullptr)
        return nullptr;
      fast = fast->next;
    }

    while (fast != nullptr) {
      fast = fast->next;
      slow = slow->next;
    }

    return slow;
  }

  /*
    1. create 2 dummy node for 2 temp linked list
    2. linked list temp1 node less than x.
    3. temp2 greater than or equal x.
    4. loop through original x.
    5. assgin next node of temp_ll1 to current node original,
follow ruler of step 2.
    6. merge the two new list dummy.
    7. update head on the lists.

  */
  void partition_list(int x) {
    if (head == nullptr)
      return;

    Node *dummy1 = new Node(0); // Node dummy1(0);
    Node *temp_dummy1 = dummy1; // Node* prev1 = &dummy1;
    Node *dummy2 = new Node(0); // Node dummy2(0)
    Node *temp_dummy2 = dummy2; // Node* prev2 = &dummy2;
    Node *current = head;

    /*
 (?) Node* dummy1 = new Node(0) vs Node dummy1(0)
 => Node dummy1(0) allocated on the stack.
 => new Node(0) is Node object that is allocated on the heap

 When to use which?:
 - Node dummy(0) is automatically allocated and also
  deallocated after function ends or block where it's declared.

 - new Node(0) is memory allocated on the head persists
 until it's explicitly deallocated.
    */

    while (current != nullptr) {
      if (current->val < x) {
        temp_dummy1->next = current;
        temp_dummy1 = current;
      } else {
        temp_dummy2->next = current;
        temp_dummy2 = current;
      }

      current = current->next;
    }

    temp_dummy2->next = nullptr;
    temp_dummy1->next = dummy2->next; // prev1->next = dummy2.next;
    head = dummy1->next;
  }

  /*
    remove all duplicate vals
  */
  void remove_duplicates_no_use_set() {
    if (head == nullptr)
      return;

    Node *current = head;
    Node *runner;

    while (current != nullptr) {
      runner = current;
      while (runner->next != nullptr) {
        if (current->val == runner->next->val) {
          runner->next = runner->next->next;
          --length;
        } else {
          runner = runner->next;
        }
      }

      current = current->next;
    }
  }

  /*
    Create an unordered set called values to store unique values
    of nodes.

    Initialize two pointers, previous and current,
    to nullptr and the head of the linked list, respectively.

    Iterate through the linked list using the current pointer:

    If the value of the current node is already in the set
    (values), update the next pointer of the previous node to point to the next
    node of the current node (thus skipping the current node) and decrement the
    length of the linked list by 1.

    If the value of the current node is not in the set,
    insert the value into the set (values) and set previous to the
    current node.

    Move the current pointer to the next node in the linked list
  */
  void remove_duplicates_use_set() {
    if (head == nullptr)
      return;

    Node *current = head;
    Node *previous = nullptr;
    unordered_set<int> values;

    while (current != nullptr) {
      // if find() did not return past-the-end end().
      // that mean value available in unordered_set values.
      if (values.find(current->val) != values.end()) {
        previous->next = current->next;
        --length;
      } else {
        values.insert(current->val);
        previous = current;
      }

      current = current->next;
    }
  }

  void print_unordered_set(const unordered_set<int> &set) {
    for (const int &elem : set) {
      std::cout << elem << ' ';
    }

    std::cout << '\n';
  }

  /*
    - Description:
    You have a linked list where each node represents a binary
    digit (0 or 1). The goal of the binaryToDecimal function is to convert this
    binary number, represented by the linked list, into its decimal equivalent.
  */
  int binary_to_decimal() {
    int result = 0;
    Node *current = head;

    while (current != nullptr) {
      result = result * 2 + current->val;
      current = current->next;
    }

    return result;
  }

  /*
    reverses the nodes of the list from the indexes m to n (the
    positions are 0-indexed). Input: 1 -> 2 -> 3 -> 4 -> 5 =>
    reverse_between(1, 3) Output: 1 -> 4 -> 3 -> 2 -> 5

  */
  void reverse_between(int m, int n) {
    if (head == nullptr)
      return;

    Node *dummy = new Node(0);
    dummy->next = head;
    Node *prev = dummy;

    for (int i = 0; i < m; i++) {
      prev = prev->next;
    }

    Node *current = prev->next;
    for (int i = 0; i < n - m; i++) {
      Node *temp = current->next;
      current->next = temp->next;
      temp->next = prev->next; // Segmentation fault: temp->next = current;
      prev->next = temp;
    }

    head = dummy->next;
    delete dummy;
    print_list();
  }
};
