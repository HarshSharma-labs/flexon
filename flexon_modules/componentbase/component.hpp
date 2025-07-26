#ifndef __FLEXON_COMPONENT_BUILDER__
#define __FLEXON_COMPONENT_BUILDER__

#include "../utilities/defs.hpp"
#include <cstdint>

typedef void (*press)();

// defination to access color value
typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
} colornc;

typedef struct {
  float top;
  float bottom;
  float left;
  float right;
} uniparam;

typedef struct {
  float height;
  float width;
  float flex;
  float gap;
  int overflow;
} lay;

typedef struct {
} onEvent;
typedef struct {
  uint32_t *backgroundColor;
  uint32_t *borderColor;
  uint8_t bgColorCount;
  uint8_t borderColorCount;
} __color;

typedef struct {
  uniparam margin;
  uniparam padding;
  uniparam Border;
  __color color;
  enum alignTypes alignitems;
  enum justifyTypes justifyItems;
  enum positionTypes position;
} decorator;

struct baseView {
  lay layout;
  decorator style;
  baseView *parent{nullptr};
  baseView *child{nullptr};
  baseView *neighbour{nullptr};
};

typedef struct {
  struct baseView relativeTree;
  struct baseView absoluteTree;
} flexonGuiTree;
typedef struct baseView flexonView;

#endif // !__FLEXON_COMPONENT_BUILDER__
