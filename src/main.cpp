#include "../flexon_modules/components/View.hpp"
#include "../flexon_modules/flexon.hpp"
#include "../flexon_modules/utilities/units.hpp"
int main() {
  flexon([]() {
    View([]() {
      Modifier
          .Color("%bl %bt %bb", hex(0xcf00ff21), hex(0xffffffff),
                 hex(0xcfffcfac))
          .space("%pb %pt", 10.0f, 100.0f)
          .setFlex(1.0f)
          .Dimensions(vh(10.0f), 20.0f)
          .Height(vh(10.0f));
    });
  });
  return 0;
}
