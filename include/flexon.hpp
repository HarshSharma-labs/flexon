#ifndef __FLEXON_MAIN_ENTRY__
#define __FLEXON_MAIN_ENTRY__

#include "../renderer/GUI/flexonGuiRenderer.hpp"
#include "flexon/view.hpp"
#include <GLFW/glfw3.h>
#include <functional>

typedef flexonView *&mine;
typedef flexonView *&child;
extern flexonView **tmp;

class flexon : private flexonRender {
public:
  flexon(std::function<void(flexonView **&)>);

private:
  void flexonInitGraphics(void);
  flexonView root;
  graphics flexonGraphics = {.flexonWindow = nullptr,
                             .flexonAppName = (char *)"hello flexon",
                             .windowHeight = 500,
                             .windowWidth = 500};
  GLFWwindow *flexonContextGui = {nullptr};
  SHADER::viewShaderProgram fragShader;
};

#endif
