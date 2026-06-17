#include <forward_list> // singly linked list: std::forward_list
#include <iostream>
#include <list> // doubly linked list: std::list
/*
  ----------        ----------
  | Data   |        | Data   |
  |--------|        |--------|
  | Pointer| - - -> | Pointer|
  ----------        ----------
*/

template <typename T> struct Raw_Node {
  Raw_Node<T> *next;
  T val;
  Raw_Node<T>(T val);
};

template <typename T> class LinkedList_RawNode {
private:
  Raw_Node<T> *head = nullptr;

public:
  LinkedList_RawNode(const T &val) { head = new Raw_Node<T>(val); }

  ~LinkedList_RawNode() {
    Raw_Node<T> *current = head;
    while (current != nullptr) {
      /* === Error ===
      head = head->next; // This is the problem!
      delete current;
      current = head; // Now current points to the new head (next node)
      */

      Raw_Node<T> *next_node = current->next; // Save the next node
      delete current;                         // Delete the current node
      current = next_node;                    // Move to the next node
    }
    head = nullptr;
  }

  void prepend(const T &new_data) {
    Raw_Node<T> *new_node = new Raw_Node<T>(new_data);
    new_node->next = head;
    head = new_node;
  }

  void append(const T &new_data) {
    Raw_Node<T> *new_node = new Raw_Node<T>(new_data);
    if (head == nullptr) {
      head = new_node;
    } else {
      Raw_Node<T> *current = head;
      while (current->next != nullptr) {
        current = current->next;
      }
      current->next = new_node;
    }
  }

  void insert(int index, const T &new_data) {
    if (index < 0) {
      return;
    }

    if (index == 0) {
      prepend(new_data);
      return;
    }

    Raw_Node<T> *current = head;
    for (int i = 0; i < index - 1 && current != nullptr; ++i) {
      current = current->next;
    }

    if (current != nullptr) {
      Raw_Node<T> *new_node = new Raw_Node<T>(new_data);
      new_node->next = current->next;
      current->next = new_node;
    } else // add last element
    {
      // Handle the case where the index is out of bounds (e.g., append)
      append(new_data); // Or throw an exception, or simply return
    }
  }

  void print_linked_list() {
    Raw_Node<T> *current = head;
    while (current != nullptr) {
      std::cout << current->val << " (" << current << ")" << " --> ";
      current = current->next;
    }
    std::cout << "nullptr" << std::endl; // Indicate the end of the list
  }

  void delete_last() {
    if (head == nullptr)
      return;

    if (head->next == nullptr) // Only one element in the list
    {
      delete head;
      head = nullptr;
      return;
    }

    Raw_Node<T> *current = head;
    Raw_Node<T> *prev = nullptr; // Keep track of the previous node

    while (current->next != nullptr) {
      prev = current;
      current = current->next;
    }

    prev->next = nullptr;
    delete current;
  }

  void delete_first() {
    if (head == nullptr)
      return;
    Raw_Node<T> *temp = head; // Store the current head,
    // (if not store that will lost way to access to rest of the linked list)
    head = head->next; // Move head to the next node
    delete temp;       // Delete the old head
  }

  void reverse() {
    Raw_Node<T> *temp = head;
    Raw_Node<T> *after = nullptr;
    Raw_Node<T> *before = nullptr;

    while (temp) {
      after = temp->next;
      temp->next = before;
      before = temp;
      temp = after;
    }

    head = before;
  }

  Raw_Node<T> *get(int index) {
    Raw_Node<T> *curr = head;
    while (index > 0 && curr != nullptr) {
      curr = curr->next;
      --index;
    }
    std::cout << curr << std::endl;
    return curr;
  }

  bool set(int index, const T &new_val) {
    Raw_Node<T> *temp = get(index);

    if (temp) {
      temp->val = new_val;
      return true;
    }
    return false;
  }
};

class DoublyLinkedList {
public:
  void doubly_linked_list() {
    // Create a list containing integers
    std::list<int> l = {7, 5, 16, 8};

    // Add an integer to the front of the list
    l.push_front(25);
    // Add an integer to the back of the list
    l.push_back(13);

    // Insert an integer before 16 by searching
    auto it = std::find(l.begin(), l.end(), 16);
    if (it != l.end())
      l.insert(it, 42);

    // Print out the list
    std::cout << "l = { ";
    for (int n : l)
      std::cout << n << ", ";
    std::cout << "};\n";
  }
};

int main() {
  LinkedList_RawNode<int> list(1);
  list.append(2);
  list.append(3);
  list.prepend(0);
  list.insert(2, 5);
  list.print_linked_list();
  list.get(2);
  list.set(2, 6);
  list.print_linked_list();

  return 0;
}
