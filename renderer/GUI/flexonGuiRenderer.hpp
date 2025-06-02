#ifndef __FLEXON_GUI_RENDERER__
#define __FLEXON_GUI_RENDERER__

#include "../../include/flexon/view.hpp"
#include "shaderwrapper.hpp"
#include <GLFW/glfw3.h>
typedef struct {
  GLFWwindow *flexonWindow;
  char *flexonAppName;
  int windowHeight;
  int windowWidth;
} graphics;

class flexonRender {
public:
  void startGui(graphics *, flexonView *, SHADER::viewShaderProgram *);
  // void drawNode(flexonView *, SHADER::viewShaderProgram *);
  void calculatelayout(flexonView *, layout *, layout *);
  // void key_callback(GLFWwindow *, int, int, int, int);
  // void resize_callback(GLFWwindow *, int, int);
  // void maximize_callback(GLFWwindow *, int);
  bool initGUI(graphics *);
  SHADER::renderVBO objRender;
};
#endif // !__FLEXON_GUI_RENDERER__
