#ifndef __SHADER_WRAPPER__
#define __SHADER_WRAPPER__

#include <GL/gl.h>
#include <glm/ext/vector_float4.hpp>
#include <glm/glm.hpp>
/* !@brief vec4 color of the component
 */
#define VIEW_SHADER_COLOR_PRIMARY 0
/*
 * !@brief vec4 color of the component if gradient is activated
 */
#define VIEW_SHADER_COLOR_SECOUNDARY 1
/*  !@brief boxSize configration
 *  vec4 x = width , y = height , z = xpositon , w = yposition
 */
#define VIEW_SHADER_BOXSIZE 2
/*  !@brief BorderWidth configration
 *  vec4 x = top, y = bottom, z = left, w = right
 */

#define VIEW_SHADER_BORDERWIDTH 3
/* !brief @BorderRadius configration
 * vec4 x = topLeft, y = topRight, z = topRightBottom, w = topLeftBottom
 */
#define VIEW_SHADER_BORDERRADIUS 4
#define VIEW_SHADER_BORDER_PRIMARY_COLOR 5
#define VIEW_SHADER_BORDER_SECOUNDRY_COLOR 6
#define VIEW_BORDER_GRADIENT_POSITION 7
#define VIEW_BACKGROUND_GRADIENT_POSITION 8

namespace SHADER {

extern int countView;
GLuint loadShader();
void destroyShader();
extern const char *viewUniforms[10];
extern GLuint Viewindices[6]; // = {0, 1, 3, 1, 2, 3};

struct renderVBO {
  GLuint *VAO;
  GLuint *VBO;
  GLuint *EBO;
  int sizeVAO;
  int sizeVBO;
  int sizeEBO;
};

typedef struct {
  GLuint shaderUniformsLoc[9];

  GLuint shaderProgram;
} viewShaderProgram;

typedef struct {
  float NDC[12];
  glm::vec4 BoxSize;
  glm::vec4 borderWidth;
  glm::vec4 borderRadius;
  glm::vec4 backgroundColorPrimary;
  glm::vec4 backgroundColorSecoundary;
  glm::vec4 borderColorPrimary;
  glm::vec4 borderColorSecoundary;
  glm::vec2 borderGradientPosition;
  glm::vec2 backgroundGradientPosition;
  /* !@brief ShaderProgramId;
   * id of the compiled shader;
   */
  GLuint fragShader;
  GLuint vertexShader;
  GLuint VAO;
  GLuint VBO;
  GLuint EBO;

} VIEW;

typedef struct {
  VIEW View;
} FRAGSHADER;
int loadShaderUniform(GLuint, SHADER::viewShaderProgram *, const char *[],
                      SHADER::renderVBO *);

} // namespace SHADER

#endif
