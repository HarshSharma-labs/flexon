#ifndef __FLEXON__
#define __FLEXON__

#define RENDERER_VULKAN
#define WINDOW_MANAGER_WAYLAND

#include "./subsystem/windowManager.hpp"
#include "./subsystem/renderer/renderer.hpp"
#include "./componentbase/component.hpp"
#include "./componentbase/flexonGlobal.hpp"
#include "./utilities/cleaner.hpp"
#include "components/Decorator.hpp"
#include <cstring>
#include <type_traits>

typedef struct baseView *capture;

class flexon {
public:
  template <typename T> flexon(const char *appName,void (*root_layout_set)(), T call_child) {
    
  };

  ~flexon() {/* free_node(tree_type.relative.child); */ }

private:
 };

#endif
