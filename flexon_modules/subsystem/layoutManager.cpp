#include "../componentbase/component.hpp"
#include "../utilities/defs.hpp"
#include "./flexonSubsystems.hpp"

struct flexon_unit_factor *unit_factor_ref = nullptr;

void subsystem::layoutManager::calcLayout(flexon_gui_tree *tree) { return; }
void subsystem::layoutManager::mountFactors(struct flexon_unit_factor *mount) {
  unit_factor_ref = mount;
}
