#include <iostream>

#include "array_list.hpp"
#include "linked_list.hpp"

using namespace itis;

int main(int argc, char **argv) {
  std::cout << "Hello, stranger!" << std::endl;

  auto array_list = new ArrayList();

  delete array_list;
  return 0;
}