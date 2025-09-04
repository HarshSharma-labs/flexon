#ifndef __FLEXON_RENDERER__
#define __FLEXON_RENDERER__

#include <iostream>

#if defined(RENDERER_VULKAN)
#include "./vulkan.hpp"
 #define RENDERER_CLASS vulkan_renderer
#endif


typedef RENDERER_CLASS renderSubsystem;

#endif // !
