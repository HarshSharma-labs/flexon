#ifndef __FLEXON_SUBSYSTEM__
#define __FLEXON_SUBSYSTEM__
#include "../componentbase/component.hpp"

namespace subsystem {
namespace windowManager {
void startWM();
void showWindow(flexon_view *);
void startInput();
void renderTemp(flexon_view *);
} // namespace windowManager
namespace layoutManager {
void calcFromNode(flexon_view *);
void calcAbs(flexon_view *);
void mountDimensions(int, int);
void mountFactors(struct flexon_unit_factor *);
} // namespace layoutManager
}; // namespace subsystem
#endif
