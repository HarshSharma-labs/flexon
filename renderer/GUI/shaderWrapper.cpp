#include "./shaderwrapper.hpp"
#include <GL/gl.h>
#include <GLES3/gl3.h>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

int SHADER::countView{0};
GLuint vertexShader, fragShader;
void createVertexBuffer(SHADER::renderVBO *);

void SHADER::destroyShader() {
  glDeleteShader(vertexShader);
  glDeleteShader(fragShader);
  return;
};

void createVertexBuffer(SHADER::renderVBO *_obj) {
  GLuint *VAO, *VBO, *EBO;
  unsigned int countTotal = (unsigned int)SHADER::countView;
  _obj->sizeVAO = countTotal;
  _obj->sizeEBO = countTotal;
  _obj->sizeVBO = countTotal;
  _obj->VAO = new GLuint[1];
  _obj->VBO = new GLuint[1];
  _obj->EBO = new GLuint[1];
  std::cout << "hello jafk" << _obj->VBO << std::endl;
  // VAO = _obj->VAO;
  // VBO = _obj->VBO;
  // EBO = _obj->EBO;
  glGenVertexArrays(1, _obj->VAO);
  glGenBuffers(1, _obj->VBO);
  glGenBuffers(1, _obj->EBO);
  return;
}

int SHADER::loadShaderUniform(GLuint shaderProgram,
                              SHADER::viewShaderProgram *__UNI,
                              const char *__textuni[],
                              SHADER::renderVBO *_obj) {

  for (int i = 0; __textuni[i] != nullptr; i++) {
    __UNI->shaderUniformsLoc[i] =
        glGetUniformLocation(shaderProgram, __textuni[i]);
  }
  std::cout << "createing vertex buffer" << std::endl;
  createVertexBuffer(_obj);
  return 0;
};

GLuint SHADER::loadShader() {
  std::string vertShaderSrc;
  std::string fragShaderSrc;
  unsigned int vertexShader{0};
  unsigned int fragmentShader{0};
  GLuint glShaderProgram{0};
  const char *VERTEX_SHADER = nullptr;
  const char *FRAG_SHADER = nullptr;

  std::filesystem::path vertShaderPath =
      "/home/harsh/flexon/renderer/GUI/shaders/vert.glsl";
  std::filesystem::path fragShaderPath =
      "/home/harsh/flexon/renderer/GUI/shaders/frag.glsl";

  std::ifstream shaderfile;
  std::stringstream buffer;
  int success;
  char infolog[1024];

  // Load vertex shader
  std::cout << "Loading shader from path: " << vertShaderPath << std::endl;
  shaderfile.open(vertShaderPath);

  if (shaderfile.is_open()) {
    buffer << shaderfile.rdbuf();
    vertShaderSrc = buffer.str();
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    VERTEX_SHADER = vertShaderSrc.c_str();
    glShaderSource(vertexShader, 1, &VERTEX_SHADER, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(vertexShader, 1024, NULL, infolog);
      shaderfile.close();
      std::cout << infolog << std::endl;

      return -1;
    }

    buffer.str("");
    buffer.clear(); // reset buffer
    shaderfile.close();
  } else {
    std::cerr << "Failed to open vertex shader file.\n";
  }

  // Load fragment shader
  std::cout << "Loading shader from path: " << fragShaderPath << std::endl;
  shaderfile.open(fragShaderPath);
  if (shaderfile.is_open()) {
    buffer << shaderfile.rdbuf();
    fragShaderSrc = buffer.str();
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    FRAG_SHADER = fragShaderSrc.c_str();
    glShaderSource(fragmentShader, 1, &FRAG_SHADER, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
      glGetShaderInfoLog(fragmentShader, 1024, NULL, infolog);
      shaderfile.close();
      std::cout << infolog << std::endl;
      return -1;
    }
    shaderfile.close();
  } else {
    std::cout << "Failed to open fragment shader file.\n";
  }
  glShaderProgram = glCreateProgram();
  glAttachShader(glShaderProgram, vertexShader);
  glAttachShader(glShaderProgram, fragmentShader);
  glLinkProgram(glShaderProgram);

  glGetProgramiv(glShaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(glShaderProgram, sizeof(infolog), NULL, infolog);
    std::cerr << "Linking of shader failed (reason): " << infolog << std::endl;
    return -1;
  }
  return glShaderProgram;
};
