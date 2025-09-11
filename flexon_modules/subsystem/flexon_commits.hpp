#ifndef __FLEXON_COMMIT__
#define __FLEXON_COMMIT__

struct commit_info{
  size_t idx_size;
  size_t vert_size;
  uint32_t height;
  uint32_t width;
};

struct rcommit_info{
  uint32_t *drawn_pixel;

};

#endif
