#ifndef __FLEXON_COMMIT__
#define __FLEXON_COMMIT__
#include "../components/matrices.hpp"


struct commit_info{
  size_t idx_size;
  size_t vert_size;
  uint32_t fheight;
  uint32_t fwidth;
};

struct rcommit_info{
  uint32_t *drawn_pixel;

};


struct rcommit_wm{
 vec2<float> max;
 vec2<uint32_t> umax;
 uint32_t *pixels;
 struct wl_display* display;
 struct wl_surface* surface;
};

struct commit_wm{
  char *name;
  vec2<float> required;
  vec2<float> min;
  rcommit_wm rc;
};



#endif
