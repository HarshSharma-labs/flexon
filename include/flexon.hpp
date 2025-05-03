#ifndef __FLEXON_MAIN_ENTRY__
#define __FLEXON_MAIN_ENTRY__

#include "flexon/view.hpp"
#include <GLFW/glfw3.h>
#include <functional>

typedef flexon_view *&mine;
typedef flexon_view *&child;
extern flexon_view **tmp;

class flexon {
public:
  flexon(std::function<void(flexon_view **&)> composable) {
    tmp = &root;
    composable(tmp);
    return;
  };

private:
  void flexonInitGraphics(void);
  flexon_view *root{nullptr};
  GLFWwindow *flexonContextGui = {nullptr};
};

#endif
