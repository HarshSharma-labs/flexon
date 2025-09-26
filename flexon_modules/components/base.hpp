#ifndef __FLEXON_COMPONENT_BASE__
#define __FLEXON_COMPONENT_BASE__
#include <stdint.h>
#include "./matrices.hpp"
#include "./hook.hpp"


enum justify_types {
   JUSTIFY_FLEX_START,
   JUSTIFY_FLEX_END,
   JUSTIFY_CENTER,
   JUSTIFY_START,
   JUSTIFY_END,
   JUSTIFY_SPACE_BETWEEN,
   JUSTIFY_SPACE_AROUND,
   JUSTIFY_SPACE_EVENLY,
   JUSTIFY_GRID_START,
   JSUTIFY_GRID_END,
};

enum align_types{
  ALIGN_CENTER,
  ALIGN_BASELINE,
  ALIGN_STRETCH,
  ALIGN_FLEX_START,
  ALIGN_FLEX_END,
};

enum display_type{
  DISPLAY_FLEX = 0,
  DISPLAY_GRID = 1,
  DISPLAY_NONE = 2,
  DISPLAY_CUSTOM = 3,

};
enum layout_direction{
  LAYOUT_DIRECTION_ROW,
  LAYOUT_DIRECTION_COL
};
enum positon{
  POSITION_RELATIVE = 0,
  POSITION_ABSOLUTE,
};


enum color_fill_type {
  COLOR_LINEAR_GRAD = 0,
  COLOR_CONICAL_GRAD,
  COLOR_RADIAL_GRAD,
  COLOR_FILL,
  COLOR_SHADOW_INSET,
  COLOR_SHADOW_OUTSET,
};
enum over_flow_types {
  OVER_SHOW = 0,
  OVER_HIDDEN,
};

enum visible {
 VIEW_SHOWN,
 VIEW_HIDE,
};

enum view_update_type{

 UPDATE_TYPE_FLUSH = 1 << 0,

 UPDATE_TYPE_COLOR = 1 << 1,
 UPDATE_TYPE_GRDIENTCOLOR = 1 << 2,

 UPDATE_TYPE_GEOMETRY = 1 << 3,

 UPDATE_TYPE_BORDERWIDTH = 1 << 4,
 UPDATE_TYPE_BORDERCOLOR = 1 << 5,

 UPDATE_TYPE_MARGIN = 1 << 6,
 UPDATE_TYPE_PADDING= 1 << 7,

 UPDATE_TYPE_RADIUS = 1 << 8,

};

/*
 * [CRITICAL SECTION DON'T REORDER FIELDS]
 */

enum unitypes{
 UINTYPE_NONE = 0,
 UNITYPE_PADDING_TOP = 1 << 0,
 UNITYPE_PADDING_BOTTOM = 1 << 1,
 UNITYPE_PADDING_RIGHT = 1 << 2,
 UNITYPE_PADDING_LEFT = 1 << 3,
 
 UNITYPE_MARGIN_TOP = 1 << 4,
 UNITYPE_MARGIN_BOTTOM = 1 << 5,
 UNITYPE_MARGIN_RIGHT = 1 << 6,
 UNITYPE_MARGIN_LEFT = 1 << 7,

 UNITYPE_BORDERRADIUS_TOP_LEFT =  UNITYPE_PADDING_TOP,
 UNITYPE_BORDERRADIUS_TOP_RIGHT = UNITYPE_PADDING_BOTTOM,
 UNITYPE_BORDERRADIUS_BOTTOM_LEFT = UNITYPE_PADDING_RIGHT,
 UNITYPE_BORDERRADIUS_BOTTOM_RIGHT = UNITYPE_PADDING_LEFT,

 UNITYPE_BORDERWIDTH_TOP =  UNITYPE_MARGIN_TOP,
 UNITYPE_BORDERWIDTH_BOTTOM = UNITYPE_MARGIN_BOTTOM,
 UNITYPE_BORDERWIDTH_RIGHT = UNITYPE_MARGIN_RIGHT,
 UNITYPE_BORDERWIDTH_LEFT = UNITYPE_MARGIN_LEFT,
 UNITYPE_STRIDE_PADDING = 0,
 UNITYPE_STRIDE_MARGIN = 4,
 UNITYPE_STRIDE_BORDERRADIUS = UNITYPE_STRIDE_PADDING,
 UNITYPE_STRIDE_BORDERWIDTH = UNITYPE_STRIDE_MARGIN,

 UNITYPE_PADDING_ALL = 0x0f,
 UNITYPE_MARGIN_ALL = 0xf0, 
 UNITYPE_BORDERRADIUS_ALL = UNITYPE_PADDING_ALL,
 UNITYPE_BORDERWIDTH_ALL = UNITYPE_MARGIN_ALL
};

/*
 * [CRITICAL SECTION END]
 */


struct normal_fill{
 uint32_t type;
 uint32_t color; 
};

typedef struct gradient_color{
 gradient_color *next;
 float stops;
 uint32_t color;
}gradient_color;

struct linear_gradient{
 uint32_t type;
 vec2 <float>pstart;
 vec2 <float>pend;
 gradient_color *colors;
};

//can be also radial
struct sweep_gradient{
 uint32_t type;
 vec3 <float>position;
 gradient_color *colors;
 
};


typedef struct callback{
 void(*onPress)(void *args);
 void(*onhover)(void *args);
}callback;


typedef struct base_quad{
  // (x,y) of all four points in ndc
  mat4x2 <float> NDC;
  // SDF - eval quad
  vec4 <float> z;
}base_quad;

typedef struct base_geo{
 // x , y , width , height
 vec4 <float> dimension;
 // vec4 Border width
 vec4 <float>borderWidth;
 // vec4 Margin of the quad
 vec4 <float>margin;
 // vec4 Padding inside the View
 vec4 <float>padding;
 // vec4 For corner radius
 vec4 <float>cornerRadius;
}base_geo;



typedef struct base_paint{
   base_geo geometry;
   uint32_t *bgColor;
   uint32_t *borderColor;  
   uint32_t update_type;
   float flex;
   uint8_t visible;
   uint8_t alignitems;
   uint8_t justifyItems;
   uint8_t position;
   uint8_t overflow;
   uint8_t display;
   uint8_t layout_direction;
  /*
  * flagpadma: represent what kind of update are made to the
  *            padding and margin of a node.
  *   sizeof(flagpadmar) = 1 byte (8bits)
  *   4 bits for 4 types of padding;
  *   and other 4 for 4 types of margin;
  *   paddings  margins
  *    0000     0000
  */
   uint8_t flagpadmar;

  /*
  * flagborrad: represent what kind of update are made to the
  *            border-width and border-radius of a node.
  *   sizeof(flagborrad) = 1 byte (8bits)
  *   4 bits for 4 types of border-width;
  *   and other 4 for 4 types of border-radius;
  *   border-width  border-radius
  *       0000         0000
  */
  
   uint8_t flagborrad;
}base_paint;



typedef struct fiber{
   uint32_t fiberid;
   base_quad quad;
   base_paint *paint;
   void(*child)();
   fiber *parent;
   fiber *childNode;
   fiber *clone;
   fiber *next;
   fiber *prev;
}fiber;

#endif
