#ifndef __FLEXON_VIEW__
#define __FLEXON_VIEW__

#include <cstdint>
typedef struct {
  float width;
  float height;
  int flex : 2;
  int alignitems : 4;
  int justifycontent : 4;

} layout;

class flexonView {
public:
  flexonView() {}
  flexonView *parent{nullptr};
  flexonView *children{nullptr};
  flexonView *neighbour{nullptr};
  uint64_t view_id{0};
  bool isViewText{false};
  char *Text{nullptr};
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
