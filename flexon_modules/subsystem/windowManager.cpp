#include "../utilities/units.hpp"
#include "flexonSubsystems.hpp"
#include <GL/gl.h>
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <cstdlib>
#include <iostream>

float rect1[] = {1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
                 -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f};
unsigned int indices[] = {0, 1, 3, 1, 2, 3};
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

unsigned int vertex_shader_id;
unsigned int fragment_shader_id;
unsigned int VAOs[2], VBOs[2], EBO;

const char *vertexShader = R"(#version 330 core
   layout(location = 0) in vec3 aPos;
   void main(){
  gl_Position = vec4(aPos, 1.0);
  }
)";

const char *fragShader = R"(#version 330 core
  out vec4 FragColor;

  void main(){
    FragColor = vec4(0.0,1.0,0.0,1.0);
  }
)";

unsigned int shaderProgram;

void compileShader() {
  vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader_id, 1, &vertexShader, NULL);
  glCompileShader(vertex_shader_id);
  int success;
  char infolog[512];

  glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &success);
  glGetShaderInfoLog(vertex_shader_id, 512, NULL, infolog);
  if (!success) {
    std::cout << "vertex :" << infolog << std::endl;
  }
  fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader_id, 1, &fragShader, NULL);
  glCompileShader(fragment_shader_id);

  int f_success;
  char finfolog[512];

  glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &f_success);
  glGetShaderInfoLog(fragment_shader_id, 512, NULL, finfolog);

  if (!f_success) {
    std::cout << "shader :" << finfolog << std::endl;
  }
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertex_shader_id);
  glAttachShader(shaderProgram, fragment_shader_id);
  glLinkProgram(shaderProgram);

  int g_success;
  char sinfolog[512];

  glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &g_success);
  glGetProgramInfoLog(shaderProgram, 512, NULL, sinfolog);

  if (g_success) {
    std::cout << "shader link :" << sinfolog << std::endl;
  }

  return;
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

void subsystem::windowManager::showWindow(flexon_view *mount_layout) {
  local_window_manager.window = glfwCreateWindow(
      local_config.width, local_config.height, "hello flexon", NULL, NULL);
  glfwGetWindowSize(local_window_manager.window, &local_config.drawableWidth,
                    &local_config.drawableHeight);
  unit_factor.vhFactor =
      static_cast<float>(local_config.drawableHeight) * 0.01f;
  unit_factor.vwFactor = static_cast<float>(local_config.drawableWidth) * 0.01f;
  subsystem::layoutManager::mountFactors(&unit_factor);
  unitMountFactor(&unit_factor);
  mount_layout->layout.x = 0.0f;
  mount_layout->layout.y = 0.0f;
  mount_layout->layout.width = static_cast<float>(local_config.drawableWidth);
  mount_layout->layout.height = static_cast<float>(local_config.drawableHeight);
  glfwMakeContextCurrent(local_window_manager.window);
  glViewport(0, 0, local_config.drawableWidth, local_config.drawableHeight);
  compileShader();
  return;
}

void refresh() {
  glViewport(0, 0, local_config.drawableWidth, local_config.drawableHeight);

  glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(shaderProgram);
  glBindVertexArray(VAOs[0]);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glfwSwapBuffers(local_window_manager.window);
}
void subsystem::windowManager::renderTemp(flexon_view *node) {
  glGenVertexArrays(1, VAOs);

  glGenBuffers(1, VBOs);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAOs[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(node->gpu_vertex.vertex),
               node->gpu_vertex.vertex, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  refresh();
}
void subsystem::windowManager::startInput() {
  // refresh();
  while (!glfwWindowShouldClose(local_window_manager.window)) {
    refresh();
    glfwWaitEvents();
  }
  glDeleteProgram(vertex_shader_id);
  glDeleteProgram(fragment_shader_id);
  glfwDestroyWindow(local_window_manager.window);
  glfwTerminate();
  return;
}
