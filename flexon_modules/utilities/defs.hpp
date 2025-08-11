#ifndef __FLEXON_DEFS__
#define __FLEXON_DEFS__

enum justify_types {
  JUSTIFY_FLEX_START = 0,
  JUSTIFY_FLEX_END,
  JUSTIFY_CENTER,
  JUSTIFY_SPACE_BETWEEN,
  JUSTIFY_SPACE_AROUND,
  JUSTIFY_SPACE_EVENLY,
  JUSTIFY_START,
  JUSTIFY_END
};

enum align_types {
  ALIGN_CENTER = 0,
  ALIGN_BASELINE,
  ALIGN_STRETCH,
  ALIGN_FLEX_START,
  ALIGN_FLEX_END,
};

enum display_types { DISPLAY_GRID = 1, DISPLAY_FLEX = 0, DISPLAY_NONE = 2 };

enum layout_directions { LAYOUT_COLOUMN = 0, LAYOUT_ROW };
enum position_types {
  POSITION_ABSOLUTE = 0,
  POSITION_RELATIVE,
};

enum color_fill_type {
  COLOR_LINEAR_GRAD = 0,
  COLOR_CONICAL_GRAD,
  COLOR_RADIAL_GRAD,
  COLOR_FILL,
  COLOR_SHADOW_INSET,
  COLOR_SHADOW_OUTSET,
};
enum unit_types { UNIT_FLEX = 0, UNIT_VH, UNIT_VW, UNIT_REL, UNIT_FILL_MAX };

enum over_flow_types {
  OVER_SHOW = 0,
  OVER_HIDDEN,
};

struct flexon_unit_factor {
  float remFactor;
  float vhFactor;
  float vwFactor;
};

#endif
