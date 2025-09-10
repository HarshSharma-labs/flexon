#ifndef __FLEXON_SHADER_CODE__
#define __FLEXON_SHADER_CODE__

#include <glm/glm.hpp>

extern const char *vertex_shader_code;
extern const char *fragment_shader_code;

typedef struct entity_rectangle_vertices{
  glm::vec2 top_right;
  glm::vec2 top_left;
  glm::vec2 bottom_left;
  glm::vec2 bottom_right;
}entity_rectangle_vertices;

#endif

