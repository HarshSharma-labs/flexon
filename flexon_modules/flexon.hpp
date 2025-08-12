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

    subsystem.window.start();
    subsystem.window.show(&flexon_tree.relative_tree);
    subsystem.layout.mountWH(flexon_tree.relative_tree.layout.height,
                             flexon_tree.relative_tree.layout.width);
    global_stitch = &flexon_tree.relative_tree.child;

    call_child();
    if (flexon_tree.relative_tree.child != nullptr) {
      flexon_tree.relative_tree.child->parent = &flexon_tree.relative_tree;
    }
    subsystem.layout.calcFromNode(flexon_tree.relative_tree.child);
    subsystem.window.start_Input();
    free_node(flexon_tree.relative_tree.child);
  };

private:
  flexon_gui_tree flexon_tree;
  subsystemModule subsystem;
};

#endif
