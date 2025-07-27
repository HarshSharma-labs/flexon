#ifndef __FLEXON_COMPONENT_BUILDER__
#define __FLEXON_COMPONENT_BUILDER__

#include "../utilities/defs.hpp"
#include <cstdint>
#include <endian.h>

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
} uni_param;

typedef struct {
  float height;
  float width;
  float flex;
  uni_param margin;
  uni_param padding;
  uni_param Border;
  enum unit_types height_unit;
  enum unit_types width_unit;
} lay;

typedef struct {
} onEvent;

struct color_tree {
  colornc color;
  struct color_tree *next;
};

struct border_color {
  colornc top;
  colornc bottom;
  colornc left;
  colornc right;
};

typedef struct {
  colornc color;
  colornc bgColor;
  border_color border;
  enum color_fill_type bg_fill_type;
  enum color_fill_type color_type;
  enum color_fill_type border_fill_type;
} view_color_options;

typedef struct {
  view_color_options color;
  enum align_types alignitems;
  enum justify_types justifyItems;
  enum position_types position;
  enum over_flow_types overflow;
} decorator;

struct base_view {
  lay layout;
  decorator style;
  base_view *parent{nullptr};
  base_view *child{nullptr};
  base_view *neighbour{nullptr};
};

typedef struct {
  struct base_view relative_tree;
  struct base_view absolute_tree;
} flexon_gui_tree;
typedef struct base_view flexon_view;

#endif // !__FLEXON_COMPONENT_BUILDER__
