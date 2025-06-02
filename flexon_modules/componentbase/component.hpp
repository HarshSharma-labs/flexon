#ifndef __FLEXON_COMPONENT_BUILDER__
#define __FLEXON_COMPONENT_BUILDER__

#include <functional>

typedef void (*press)();

typedef struct {
  float r;
  float g;
  float b;
  float a;
} colornc;

typedef struct {
  colornc top;
  colornc bottom;
  colornc left;
  colornc right;
} colorncx;

typedef struct {
  float top;
  float bottom;
  float left;
  float right;
  float overall;
} uniparam;

typedef struct {
  float height;
  float width;
  float flex;
  float gap;
  int overflow;
  int alignItems;
  int justifyContent;
} lay;

struct baseView {
  lay layout;
  uniparam margin;
  uniparam padding;
  uniparam Border;
  colorncx borderColor;
  colorncx backgroundColor;
  baseView *parent{nullptr};
  baseView *child{nullptr};
  baseView *neighbour{nullptr};
  press onPress{nullptr};
  press onLongPress{nullptr};
};
typedef struct {
  struct baseView relativeTree;
  struct baseView absoluteTree;
} flexonGuiTree;
typedef struct baseView flexonView;
typedef std::function<void(flexonView *)> functionSignature;
typedef std::function<void(void)> functionSignatureVoid;

#endif // !__FLEXON_COMPONENT_BUILDER__
