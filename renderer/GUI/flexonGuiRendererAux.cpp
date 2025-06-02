#include "./flexonGuiRenderer.hpp"
#include "shaderwrapper.hpp"
#include <GL/glext.h>
#include <GLES3/gl3.h>
#include <glm/ext/vector_float4.hpp>
#include <iostream>

void calculateNDC(SHADER::VIEW *, layout *, layout *);
void bindVertex(flexonView *, SHADER::renderVBO *);

void bindVertex(flexonView *__node, SHADER::renderVBO *obj) {
  GLuint indices[] = {0, 1, 3, 1, 2, 3};
  float sizeRect[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

  glBindVertexArray((*obj).VAO[0]);

  glBindBuffer(GL_ARRAY_BUFFER, obj->VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(sizeRect), (*__node).shaderData.NDC,
               GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*obj).EBO[0]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  (*__node).shaderData.VAO = (*obj).VAO[0];
  (*__node).shaderData.VBO = (*obj).VAO[0];
  (*__node).shaderData.EBO = (*obj).VAO[0];

  return;
}
void flexonRender::calculatelayout(flexonView *__node, layout *_parentlay,
                                   layout *rootLay) {
  float netflex = 0.0f;

  for (flexonView *__tmp = __node; __tmp; __tmp = __tmp->neighbour)
    netflex += __tmp->lay.flex;

  for (flexonView *__tmp = __node; __tmp; __tmp = __tmp->neighbour) {
    __tmp->lay.height = (__tmp->lay.flex / netflex) * _parentlay->height;
    __tmp->lay.width = _parentlay->width;
    __tmp->lay.x = 0.0f; //_parentlay->x;
    __tmp->lay.y = 0.0f; //_parentlay->y;
    calculateNDC(&(__tmp->shaderData), &(__tmp->lay), rootLay);
    for (int i = 0; i < 12; i++)
      std::cout << __tmp->shaderData.NDC[i] << std::endl;

    std::cout << __tmp->shaderData.BoxSize.x << " : "
              << __tmp->shaderData.BoxSize.y << " : "
              << __tmp->shaderData.BoxSize.z << " : "
              << __tmp->shaderData.BoxSize.w << std::endl;

    bindVertex(__tmp, &objRender);
    calculatelayout(__tmp->children, &(__tmp->lay), rootLay);
  }

  return;
}

void calculateNDC(SHADER::VIEW *shade, layout *lay, layout *root) {
  float height = lay->height;
  float width = lay->width;
  float windowWidth = root->width;
  float windowHeight = root->height;
  float x = lay->x;
  float y = lay->y;
  shade->NDC[0] = (2.0f * (x + width)) / windowWidth - 1;
  shade->NDC[1] = 1 - (2 * y) / windowHeight;

  shade->NDC[3] = (2 * (x + width)) / windowWidth - 1;
  shade->NDC[4] = 1 - 2 * (y + height) / windowHeight;

  shade->NDC[6] = (2 * x) / windowWidth - 1;
  shade->NDC[7] = 1 - (2 * (y + height)) / windowHeight;

  shade->NDC[9] = (2 * x) / windowWidth - 1;
  shade->NDC[10] = 1 - (2 * y) / windowHeight;
  shade->BoxSize.x = width;
  shade->BoxSize.y = height;
  shade->BoxSize.z = (x + width) / 2.0f;
  shade->BoxSize.w = (y + height) / 2.0f;
  return;
}
