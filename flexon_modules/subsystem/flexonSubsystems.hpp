#ifndef __FLEXON_SUBSYSTEM__
#define __FLEXON_SUBSYSTEM__
#include "../componentbase/component.hpp"

namespace subsystem {
namespace windowManager {
void startWM();
void showWindow();
void startInput();
} // namespace windowManager
namespace layoutManager {
void calcLayout(flexon_gui_tree *);
void calcFromNode(flexon_view *);
void calcAbs(flexon_view *);
void mountFactors(struct flexon_unit_factor *);
} // namespace layoutManager
}; // namespace subsystem
#endif
