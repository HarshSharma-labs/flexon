#include "./cleaner.hpp"
#include <iostream>

void free_node(flexon_view *starting_node) {
  if (starting_node->child != nullptr) {
    starting_node->child->clean_ptr(starting_node->child);
  }
  if (starting_node->neighbour != nullptr) {

    starting_node->neighbour->clean_ptr(starting_node->neighbour);
  }

  std::cout << " Got request to clean :" << starting_node << std::endl;

  delete starting_node;
}
