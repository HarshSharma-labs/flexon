#ifndef __FLEXON_MAIN_ENTRY__
#define __FLEXON_MAIN_ENTRY__

#include "../renderer/GUI/flexonGuiRenderer.hpp"
#include "flexon/view.hpp"
#include <GLFW/glfw3.h>
#include <functional>

typedef flexon_view *&mine;
typedef flexon_view *&child;
extern flexon_view **tmp;

class flexon : private flexonRender {
public:
  flexon(std::function<void(flexon_view **&)>);

private:
  void flexonInitGraphics(void);
  flexon_view *root{nullptr};
  graphics flexonGraphics = {.flexonWindow = nullptr,
                             .flexonAppName = (char *)"hello flexon",
                             .windowHeight = 500,
                             .windowWidth = 500};
  GLFWwindow *flexonContextGui = {nullptr};
};

#endif
