#ifndef __FLEXON__
#define __FLEXON__

#include "./componentbase/component.hpp"
#include "./componentbase/flexonGlobal.hpp"
#include "./subsystem/flexonSubsystems.hpp"
#include "./utilities/cleaner.hpp"
#include "components/Decorator.hpp"
#include <cstring>
#include <type_traits>

typedef struct baseView *capture;

class flexon {
public:
  template <typename T> flexon(void (*root_layout_set)(), T call_child) {
    static_assert(std::is_invocable<T>::value,
                  "flexon(arg): arg must be a lambda function");
    memset(&flexon_tree.relative_tree, '\0', sizeof(flexon_view));
    Modifier.mount(&flexon_tree.relative_tree);
    root_layout_set();

    subsystem::windowManager::startWM();
    subsystem::windowManager::showWindow(&flexon_tree.relative_tree);
    subsystem::layoutManager::mountDimensions(
        flexon_tree.relative_tree.layout.height,
        flexon_tree.relative_tree.layout.width);
    global_stitch = &flexon_tree.relative_tree.child;

    call_child();
    if (flexon_tree.relative_tree.child != nullptr) {
      flexon_tree.relative_tree.child->parent = &flexon_tree.relative_tree;
    }
    subsystem::layoutManager::calcFromNode(flexon_tree.relative_tree.child);
    subsystem::windowManager::renderTemp(flexon_tree.relative_tree.child);
    subsystem::windowManager::startInput();
    free_node(flexon_tree.relative_tree.child);
  };

private:
  flexon_gui_tree flexon_tree;
};

#endif
