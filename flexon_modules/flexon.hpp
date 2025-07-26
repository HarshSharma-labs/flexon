#ifndef __FLEXON__
#define __FLEXON__

#include "./componentbase/component.hpp"
#include "./componentbase/flexonGlobal.hpp"
#include "./subsystem/flexonSubsystems.hpp"
#include <type_traits>

typedef struct baseView *capture;

class flexon {
public:
  template <typename T> flexon(T callchild) {
    static_assert(std::is_invocable<T>::value,
                  "flexon(arg): arg must be a lambda function");
    subsystem::windowManager::startWM();
    globalStitch = &flexonTree.relativeTree.child;
    callchild();
  };

private:
  flexonGuiTree flexonTree;
};

#endif
