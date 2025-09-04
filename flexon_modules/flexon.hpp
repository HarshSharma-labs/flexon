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
    windowManager.create(appName);
    renderer.initlise();
    renderer.mount_surface(windowManager.getSurfacePointer(),windowManager.getDisplayPointer(),windowManager.getpixel()); 
 
    renderer.render();
    windowManager.dispatchEvent();
    renderer.destroy_renderer();
    windowManager.destroy();
/*
   static_assert(std::is_invocable<T>::value,
                  "flexon(arg): arg must be a lambda function");
    memset(&tree_type.relative, '\0', sizeof(flexon_view));

    Modifier.mount(&tree_type.relative);

    global_stitch = &tree_type.relative.child;
    global_parent_stitch = &tree_type.relative;

    start_subsystems_window();
    call_child();
    
    start_rendering();
    */
  };

  ~flexon() {/* free_node(tree_type.relative.child); */ }

private:
  void start_subsystems_window() {
   // subsystem.window.getRootLayout(&tree_type.relative);
    //subsystem.layout.mountWH(tree_type.relative.layout.height,tree_type.relative.layout.width);
  }
  void start_rendering() {
   // subsystem.layout.calcFromNode(tree_type.relative.child);
    //subsystem.window.startInput();
  };

  flexon_gui_tree tree_type;
  windowManagerSubsystem windowManager;
  renderSubsystem renderer;
  };

#endif
