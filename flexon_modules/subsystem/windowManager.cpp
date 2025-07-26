#include "flexonSubsystems.hpp"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <cstdlib>

struct flexonWindowManager {
  GLFWmonitor *monitor;
  const GLFWvidmode *monitorMode;
  GLFWwindow *window;
};

struct windowManagerConfig {
  int width;
  int height;
  int drawableHeight;
  int drawableWidth;
  float refreshRate;
};

struct flexonWindowManager localWindowManager;
struct windowManagerConfig localConfig;

void subsystem::windowManager::startWM() {
  if (!glfwInit())
    assert(false);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
  glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

  localWindowManager.monitor = glfwGetPrimaryMonitor();
  localWindowManager.monitorMode = glfwGetVideoMode(localWindowManager.monitor);
  localConfig.height = localWindowManager.monitorMode->height;
  localConfig.width = localWindowManager.monitorMode->width;
  localConfig.refreshRate = localWindowManager.monitorMode->refreshRate;

  return;
}

void subsystem::windowManager::showWindow() {
  localWindowManager.window = glfwCreateWindow(
      localConfig.width, localConfig.height, "hello flexon", NULL, NULL);
  glfwGetWindowSize(localWindowManager.window, &localConfig.drawableWidth,
                    &localConfig.drawableHeight);

  glfwMakeContextCurrent(localWindowManager.window);
  return;
}

void subsystem::windowManager::startInput() {
  while (!glfwWindowShouldClose(localWindowManager.window)) {

    glfwWaitEvents();
  }

  glfwDestroyWindow(localWindowManager.window);
  glfwTerminate();
  return;
}
