#include "../flexon_modules/components/View.hpp"
#include "../flexon_modules/flexon.hpp"
#include "../flexon_modules/utilities/units.hpp"

int main() {
  flexon("Hello app",[]() -> void {},
         []() {
           View([]() {
             Modifier
                 .Color("%bl %bt %bb", hex(0xcf00ff21), hex(0xffffffff),
                        hex(0xcfffcfac))
                 .space("%m", 1.0f)
                 .setFlex(1.0f);
           });
           View([]() {
             Modifier
                 .Color("%bl %bt %bb", hex(0xcf00ff21), hex(0xffffffff),
                        hex(0xcfffcfac))
                 .space("%m", 1.0f)
                 .setFlex(1.0f);
           });
         });

  return 0;
}
