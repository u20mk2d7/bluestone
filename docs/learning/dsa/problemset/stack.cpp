#include <iostream>
#include <stdexcept>

#include "linked_list.cpp"

template <typename T> class Stack_Node {
private:
  Raw_Node<T> *top;

public:
  Stack_Node<T>(const T &new_data) {
    Raw_Node<T> *new_node = new Raw_Node<T>(new_data);
    top = new_node;
  }

  bool isFull() {
    return false; // Stacks implemented with linked lists are never full
                  // (unless memory exhaustion)
  }

  bool isEmpty() {
    return top == nullptr; // More concise way to check for emptiness
  }

  void push(const T &new_data) // append linked list
  {
    Raw_Node<T> *new_node = new Raw_Node<T>(new_data);
    new_node->next = top;
    top = new_node;
  }

  T pop() {
    if (isEmpty()) {
      throw std::runtime_error("Cannot pop from an empty stack");
    }

    T data = top->value;
    Raw_Node<T> *temp = top;
    top = top->next;
    delete temp;
    return data;
  }
};

class Stack {
private:
  int size;
  int top;
  int *arr;

public:
  Stack(int size) {
    int size = size;
    arr = new int[size];
    top = -1;
  }

  ~Stack() { delete[] arr; }

  void push(int new_data) {
    if (top >= size - 1) {
      std::cout << "Stack Overflow!" << std::endl;
      return;
    }
    arr[++top] = new_data;
  }

  int pop() {
    if (isEmpty()) {
      std::cout << "Stack Underflow!" << std::endl;
      return -1;
    }
    return arr[top--];
  }

  bool isEmpty() { return top < 0; }
}
