#define FLEXON_ENABLE_FLEX_LAYOUT
#include "../flexon_modules/components/View.hpp"
#include "../flexon_modules/flexon.hpp"
#include "../flexon_modules/utilities/units.hpp"
int main() {

  flexon([]() { View([]() { Modifier.Color("%c", hex(0x00ffffff)); }); });

  return 0;
}
