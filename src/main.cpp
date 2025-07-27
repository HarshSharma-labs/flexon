#include "../flexon_modules/components/View.hpp"
#include "../flexon_modules/flexon.hpp"
#include "../flexon_modules/utilities/units.hpp"
int main() {
  flexon([]() {
    View([]() {
      Modifier
          .Color("%c %bg %b", hex(0x00000000), hex(0xffffffff), hex(0xcfffcfac))
          .space("%pb %pt", 10.0f, 100.0f)
          .setFlex(1.0f)
          .Dimensions(vh(10.0f), 20.0f);
    });
  });
  return 0;
}
