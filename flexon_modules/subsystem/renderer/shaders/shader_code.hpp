#ifndef __FLEXON_SHADER_CODE__
#define __FLEXON_SHADER_CODE__

#include "../../../components/base.hpp"

#define NORMAL_SHADER_VERTEX_SIZE 20
#define BOX_SHADER_VERTEX_SIZE 20

extern const char *normalShaderCodeVertex;
extern const char *normalShaderCodeFragment;

typedef struct vertexshaderdata{
  vec2 <float>top_right;
  vec2 <float>top_left;
  vec2 <float>bottom_left;
  vec2 <float>bottom_right;
}vertexshaderdata;

typedef struct normalshaderuniform {
 vec4 <float> color;
}normalshaderuniform;
#endif

