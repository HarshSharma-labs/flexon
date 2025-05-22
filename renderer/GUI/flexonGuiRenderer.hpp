#ifndef __FLEXON_GUI_RENDERER__
#define __FLEXON_GUI_RENDERER__

#include "../../include/flexon/view.hpp"
#include <GLFW/glfw3.h>
typedef struct {
  GLFWwindow *flexonWindow;
  char *flexonAppName;
  int windowHeight;
  int windowWidth;
} graphics;

class flexonRender {
public:
  void Render(graphics *);
  void calculatelayout(flexonView *, layout *);
  // void key_callback(GLFWwindow *, int, int, int, int);
  // void resize_callback(GLFWwindow *, int, int);
  // void maximize_callback(GLFWwindow *, int);
  bool initGUI(graphics *);
};
#endif // !__FLEXON_GUI_RENDERER__
