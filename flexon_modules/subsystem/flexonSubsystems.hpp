#ifndef __FLEXON_SUBSYSTEM__
#define __FLEXON_SUBSYSTEM__
#include "../componentbase/component.hpp"


namespace subsystem {
	   namespace windowManager {
        void startWM();
	      void showWindow();
	      void startInput();	
	} // namespace windowManager
	   namespace layoutManager {
      void calcLayout(flexonGuiTree *);
      void calcFromNode(flexonView *);
      void calcAbs(flexonView *);
     }
}; //namespace subsystem
#endif
