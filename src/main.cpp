#include "../flexon_modules/subsystem/windowManager/wayland.hpp"
#include "../flexon_modules/flexon.hpp"

int main() {
  waylandWM window_manager(flexon::window);
  window_manager.dispatchEvent();
  statemanager manager;
  manager.dispatchEvents();

  return 0;

}
