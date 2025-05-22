#include "flexon.hpp"

void flexon::flexonInitGraphics(void) {
  if (initGUI(&flexonGraphics)) {
    flexonContextGui = flexonGraphics.flexonWindow;
    root.lay.height = flexonGraphics.windowHeight;
    root.lay.width = flexonGraphics.windowWidth;
    Render(&flexonGraphics);
    // calculatelayout(&root, &root.lay);
  }
  return;
}

flexon::flexon(std::function<void(flexonView **&)> composable) {
  tmp = &root.children;
  composable(tmp);
  flexonInitGraphics();
  return;
}
