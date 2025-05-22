#ifndef __SHADER_WRAPPER__
#define __SHADER_WRAPPER__

#include <GL/gl.h>

#define VIEW_SHADER 0

typedef struct {
  int shaderType;
  GLuint glShaderProgram;
} flexonShader;

#endif
