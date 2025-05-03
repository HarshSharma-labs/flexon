#include "flexonGuiRenderer.hpp"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <iostream>

bool initGUI(graphics *param) {
  if (!glfwInit())
    assert(false);
  param->flexonWindow =
      glfwCreateWindow(param->windowWidth, param->windowHeight,
                       param->flexonAppName, NULL, NULL);

  return true;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  std::cout << "hello Pressed Key : " << (char)key << std::endl;
}

flexonRender::flexonRender() {

  if (!glfwInit()) {
    return;
  }
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

  GLFWwindow *window = glfwCreateWindow(800, 800, "flexon", NULL, NULL);
  glfwMakeContextCurrent(window);
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(1.0f, 0.5f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glfwSetKeyCallback(window, &key_callback);
  glfwSwapBuffers(window);
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();
}

flexonRender::~flexonRender() { return; }
