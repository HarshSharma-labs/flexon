#ifndef __FLEXON__
#define __FLEXON__
#define RENDERER_VULKAN
#define WINDOW_MANAGER_WAYLAND

#include "./components/base.hpp"
#include "./subsystem/flexon_commits.hpp"

enum windowtype{
  WINDOW_FULL_SCREEN = 1,
  WINDOW_FULL_TOPBAR_SHOWN,
  WINDOW_CUSTOM_SIZE,
};
typedef struct window{
 enum windowtype flag;
 char *name;
 uint32_t width;
 uint32_t height;
}window;


namespace flexon{
 extern void appmain();
 extern window window; 
 void start(struct commit_wm *commit);
 void dispatchCallback();
 void exit();
}

#endif
