#ifndef __FLEXON_WINDOW_MANAGER__
#define __FLEXON_WINDOW_MANAGER__
#include "../componentbase/component.hpp"
#include <GLFW/glfw3.h>
#include <cassert>

class windowManagerSubsystem {

public:
  void start() {
    if (!glfwInit())
      assert(false);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

    monitor = glfwGetPrimaryMonitor();
    monitorMode = glfwGetVideoMode(monitor);
    windowHeight = monitorMode->height;
    windowWidth = monitorMode->width;
    refreshRate = monitorMode->refreshRate;
    unit_factor.remFactor = 16.0f;
    return;
  };
  void show(flexon_view *mount_layout) {
    window =
        glfwCreateWindow(windowWidth, windowHeight, "hello flexon", NULL, NULL);
    glfwGetWindowSize(window, &drawableWidth, &drawableHeight);
    unit_factor.vhFactor = static_cast<float>(drawableHeight) * 0.01f;
    unit_factor.vwFactor = static_cast<float>(drawableWidth) * 0.01f;

    mount_layout->layout.x = 0.0f;
    mount_layout->layout.y = 0.0f;
    mount_layout->layout.width = static_cast<float>(drawableWidth);
    mount_layout->layout.height = static_cast<float>(drawableHeight);
    glfwMakeContextCurrent(window);
    return;
  };
  void start_Input() {
    while (!glfwWindowShouldClose(window)) {
      glfwWaitEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return;
  };

private:
  GLFWmonitor *monitor{nullptr};
  const GLFWvidmode *monitorMode{nullptr};
  GLFWwindow *window{nullptr};
  struct flexon_unit_factor unit_factor;
  int windowWidth;
  int windowHeight;
  int drawableHeight;
  int drawableWidth;
  float refreshRate;
};

#endif
