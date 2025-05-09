#include "flexon.hpp"
void flexon::flexonInitGraphics(void) {
  if (initGUI(&flexonGraphics)) {
    flexonContextGui = flexonGraphics.flexonWindow;
    Render(&flexonGraphics);
  }
  return;
}

flexon::flexon(std::function<void(flexon_view **&)> composable) {
  tmp = &root;
  composable(tmp);
  flexonInitGraphics();
  return;
}
