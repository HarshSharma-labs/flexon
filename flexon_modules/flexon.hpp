#ifndef __FLEXON__
#define __FLEXON__
#define RENDERER_VULKAN
#define WINDOW_MANAGER_WAYLAND

#include "./components/base.hpp"
#include "./subsystem/flexon_commits.hpp"




namespace flexon{
 void app_main();
 void post_startup(fiber *wrap , vec2<float> &windowDimension);

 void start(struct commit_wm *commit);
 void dispatchCallback();
 void exit();
}

#endif
