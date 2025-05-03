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

class flexon_view {
public:
  flexon_view() {}
  flexon_view *parent{nullptr};
  flexon_view *children{nullptr};
  flexon_view *neighbour{nullptr};
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
} term_prop;

class flexon_view_tui {
public:
private:
  term_prop layout;
};

#endif
