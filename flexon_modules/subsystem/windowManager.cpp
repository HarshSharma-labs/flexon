#include "../utilities/units.hpp"
#include "flexonSubsystems.hpp"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <cstdlib>
#include <iostream>

struct flexon_window_manager {
  GLFWmonitor *monitor;
  const GLFWvidmode *monitorMode;
  GLFWwindow *window;
};

struct window_manager_config {
  int width;
  int height;
  int drawableHeight;
  int drawableWidth;
  float refreshRate;
};

struct flexon_window_manager local_window_manager;
struct window_manager_config local_config;
struct flexon_unit_factor unit_factor;

void subsystem::windowManager::startWM() {
  if (!glfwInit())
    assert(false);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
  glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

  local_window_manager.monitor = glfwGetPrimaryMonitor();
  local_window_manager.monitorMode =
      glfwGetVideoMode(local_window_manager.monitor);
  local_config.height = local_window_manager.monitorMode->height;
  local_config.width = local_window_manager.monitorMode->width;
  local_config.refreshRate = local_window_manager.monitorMode->refreshRate;
  unit_factor.remFactor = 16.0f;
  return;
}

void subsystem::windowManager::showWindow() {
  local_window_manager.window = glfwCreateWindow(
      local_config.width, local_config.height, "hello flexon", NULL, NULL);
  glfwGetWindowSize(local_window_manager.window, &local_config.drawableWidth,
                    &local_config.drawableHeight);
  unit_factor.vhFactor =
      static_cast<float>(local_config.drawableHeight) * 0.01f;
  unit_factor.vwFactor = static_cast<float>(local_config.drawableWidth) * 0.01f;
  subsystem::layoutManager::mountFactors(&unit_factor);
  unitMountFactor(&unit_factor);

  glfwMakeContextCurrent(local_window_manager.window);

  glfwDestroyWindow(local_window_manager.window);
  glfwTerminate();
  return;
}

void subsystem::windowManager::startInput() {
  while (!glfwWindowShouldClose(local_window_manager.window)) {

    glfwWaitEvents();
  }

  glfwDestroyWindow(local_window_manager.window);
  glfwTerminate();
  return;
}
