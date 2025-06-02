#include "../flexon_modules/components/View.hpp"
#include "../flexon_modules/flexon.hpp"
#include <iostream>
int main() {

  flexon([](void) {
    View([](void) { std::cout << "Hello flexon" << std::endl; });
    View([](capture mine) {
      flexonModifier(mine).printHello();
      std::cout << "Hello flexon" << std::endl;
    });
  });

  return 0;
}
