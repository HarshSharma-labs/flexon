#ifndef __FLEXON_COMMIT__
#define __FLEXON_COMMIT__

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
 uint32_t fheight;
 uint32_t fwidth;
 uint32_t *pixels;
 struct wl_display* display;
 struct wl_surface* surface;
};

struct commit_wm{
  char *name;
  uint32_t width;
  uint32_t height;
  rcommit_wm rc;
};



#endif
