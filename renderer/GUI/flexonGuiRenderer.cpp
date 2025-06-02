#include "flexonGuiRenderer.hpp"
#include "shaderwrapper.hpp"
#include <GL/gl.h>
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <unistd.h>

void glrefresh(GLFWwindow *);
float genCol();
void resize_callback(GLFWwindow *, int, int);
void maximize_callback(GLFWwindow *, int);
void setUniformView(SHADER::viewShaderProgram *, SHADER::FRAGSHADER *);
graphics *tmp_graphics{nullptr};
flexonView *tmpnode;
SHADER::viewShaderProgram *tmpprogram;
float genCol() {
  float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
  return random;
}
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  if (key == GLFW_KEY_ESCAPE) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  if (action == GLFW_PRESS) {
    if (key == 'R') {
      //    glDeleteShader(vertshader);
      // glDeleteShader(fragshader);
      // glrefresh(window);
    }
    if (key == 'Q')
      glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

bool flexonRender::initGUI(graphics *param) {
  if (!glfwInit())
    assert(false);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
  glfwWindowHint(GLFW_SAMPLES, 4);
  GLFWmonitor *monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode *mode = glfwGetVideoMode(monitor);

  srand(static_cast<unsigned int>(time(nullptr)));

  param->windowHeight = mode->height;
  param->windowWidth = mode->width;
  param->flexonWindow =
      glfwCreateWindow(param->windowWidth, param->windowHeight,
                       param->flexonAppName, NULL, NULL);
  glfwMakeContextCurrent(param->flexonWindow);
  glfwSetKeyCallback(param->flexonWindow, key_callback);
  glfwSetWindowSizeCallback(param->flexonWindow, resize_callback);
  glfwSetWindowMaximizeCallback(param->flexonWindow, maximize_callback);
  glfwSetFramebufferSizeCallback(param->flexonWindow,
                                 framebuffer_size_callback);

  glViewport(0, 0, param->windowWidth, param->windowHeight);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  return true;
}
void drawNode(flexonView *__node, SHADER::viewShaderProgram *_program) {

  glUseProgram(_program->shaderProgram);

  GLuint VAO = __node->shaderData.VAO;
  glBindVertexArray(VAO);
  glUniform4f(_program->shaderUniformsLoc[VIEW_SHADER_BOXSIZE],
              __node->shaderData.BoxSize.x, __node->shaderData.BoxSize.y,
              __node->shaderData.BoxSize.z, __node->shaderData.BoxSize.w);
  glUniform4f(_program->shaderUniformsLoc[VIEW_SHADER_COLOR_PRIMARY], genCol(),
              genCol(), genCol(), 1.0f);
  glUniform4f(_program->shaderUniformsLoc[VIEW_SHADER_BORDERRADIUS], 0.0f, 0.0f,
              0.0f, 0.0f);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glfwSwapBuffers(tmp_graphics->flexonWindow);
  return;
};

void flexonRender::startGui(graphics *window, flexonView *node,
                            SHADER::viewShaderProgram *_program) {

  tmp_graphics = window;
  tmpnode = node;
  tmpprogram = _program;
  drawNode(node, _program);
  glfwSwapBuffers(window->flexonWindow);

  while (!glfwWindowShouldClose(window->flexonWindow)) {

    glfwWaitEvents();
  }

  SHADER::destroyShader();
  glfwDestroyWindow(window->flexonWindow);
  glfwTerminate();
}

void resize_callback(GLFWwindow *window, int width, int height) {
  std::cout << width << " : " << height << std::endl;
  glfwSetWindowSize(window, width, height);
  tmp_graphics->windowWidth = width;
  tmp_graphics->windowHeight = height;
  drawNode(tmpnode, tmpprogram);

  return;
}
void maximize_callback(GLFWwindow *window, int maximized) {
  std::cout << "maximized" << maximized << std::endl;
  drawNode(tmpnode, tmpprogram);

  return;
}
