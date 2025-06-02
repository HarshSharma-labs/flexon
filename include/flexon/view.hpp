#ifndef __FLEXON_VIEW__
#define __FLEXON_VIEW__

#include "../../renderer/GUI/shaderwrapper.hpp"
#include <cstdint>

typedef struct {
  float width;
  float height;
  float x;
  float y;
  float flex;
  int alignitems;
  int justifycontent;

} layout;

class flexonView {
public:
  flexonView *parent{nullptr};
  flexonView *children{nullptr};
  flexonView *neighbour{nullptr};
  uint64_t view_id{0};
  bool isViewText{false};
  char *Text{nullptr};

  SHADER::VIEW shaderData;
  layout lay;
};

typedef struct tflag {
  int height;
  int width;
  int flex : 2;
  int alignitems : 4;
  int justifycontent : 4;
  int backgroundColor;
  int foregroundColor;
} termProp;

class flexonViewTui {
public:
private:
  termProp layout;
};

#endif
