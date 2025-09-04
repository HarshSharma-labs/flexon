#ifndef __FLEXON_WINDOW_MANAGER__
#define __FLEXON_WINDOW_MANAGER__
#include <iostream>


#if defined(WINDOW_MANAGER_WAYLAND)
   #include "./windowManager/wayland.hpp"   
   #define WINDOW_MANAGER waylandWM
#endif

typedef WINDOW_MANAGER windowManagerSubsystem;

#endif
