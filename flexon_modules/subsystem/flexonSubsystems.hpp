#ifndef __FLEXON_SUBSYSTEM__
#define __FLEXON_SUBSYSTEM__

#include "layoutManager.hpp"
#include "windowManager.hpp"

class subsystemModule {
public:
  windowManagerSubsystem window;
  layoutModule layout;
};
#endif
