#include <iostream>

template <class T> struct Node {
  T data;
  Node *next_node;

  Node(T val) : data(val), next_node(nullptr) {}

}

template <typename T>
class Queue {
private:
  T data;
  Node *next_node;
}
