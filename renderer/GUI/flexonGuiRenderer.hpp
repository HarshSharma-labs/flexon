#ifndef __FLEXON_GUI_RENDERER__
#define __FLEXON_GUI_RENDERER__

#include <GLFW/glfw3.h>
typedef struct {
  GLFWwindow *flexonWindow;
  char *flexonAppName;
  int windowHeight;
  int windowWidth;
} graphics;

bool initGUI(graphics *);

class flexonRender {
public:
  void Render(graphics *);
};
#endif // !__FLEXON_GUI_RENDERER__
;
