#ifndef __FLEXON__
#define __FLEXON__

#include "./componentbase/component.hpp"
#include "./componentbase/flexonGlobal.hpp"
#include "./subsystem/flexonSubsystems.hpp"
#include "./utilities/units.hpp"
#include <type_traits>

typedef struct baseView *capture;

class flexon {
public:
  template <typename T> flexon(T callchild) {
    static_assert(std::is_invocable<T>::value,
                  "flexon(arg): arg must be a lambda function");
    subsystem::windowManager::startWM();
    subsystem::windowManager::showWindow();
    global_stitch = &flexon_tree.relative_tree.child;
    callchild();
    print_node_diagonistics(flexon_tree.relative_tree.child);
  };

private:
  flexon_gui_tree flexon_tree;
};

#endif
