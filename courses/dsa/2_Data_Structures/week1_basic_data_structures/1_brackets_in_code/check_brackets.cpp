#include <iostream>
#include <stack>
#include <string>

struct Bracket {
  char type;
  int position;

  Bracket(char type, int position) : type(type), position(position) {}

  bool Matchc(char c) {
    if (type == '[' && c == ']')
      return true;
    if (type == '{' && c == '}')
      return true;
    if (type == '(' && c == ')')
      return true;
    return false;
  }
};

/*
  Balanced Brackets
  Balanced:
  + ([])[]()
  + ((({([])})))

  Unbalanced:
  + ([]]()
  +
*/

int main() {
  std::string text;
  getline(std::cin, text);

  std::stack<Bracket> opening_brackets_stack;

  for (int position = 0; position < text.length(); ++position) {
    char next = text[position];

    if (next == '(' || next == '[' || next == '{') {
      opening_brackets_stack.push(Bracket(next, position));
    }

    if (next == ')' || next == ']' || next == '}') {
      if (opening_brackets_stack.empty() ||
          !opening_brackets_stack.top().Matchc(next)) {
        // opening_brackets_stack.push(Bracket(next, position));
        std::cout << position + 1 << std::endl;
        return position + 1;

      } else if (opening_brackets_stack.top().Matchc(next)) {
        opening_brackets_stack.pop();
      }
    }
  }

  if (opening_brackets_stack.empty()) {
    std::cout << "Success\n";
  } else {
    std::cout << opening_brackets_stack.top().position + 1 << std::endl;
  }

  return 0;
}
