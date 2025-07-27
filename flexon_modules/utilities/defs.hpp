#ifndef __FLEXON_DEFS__
#define __FLEXON_DEFS__

enum justify_types {
  JUSTIFY_FLEX_START = 1,
  JUSTIFY_FLEX_END,
  JUSTIFY_CENTER,
  JUSTIFY_SPACE_BETWEEN,
  JUSTIFY_SPACE_AROUND,
  JUSTIFY_SPACE_EVENLY,
  JUSTIFY_START,
  JUSTIFY_END
};

enum align_types {
  ALIGN_CENTER = 1,
  ALIGN_BASELINE,
  ALIGN_STRETCH,
  ALIGN_FLEX_START,
  ALIGN_FLEX_END,
};

enum position_types {
  POSITION_ABSOLUTE = 1,
  POSITION_RELATIVE,
};

enum color_fill_type {
  LINEAR_GRAD = 1,
  CONICAL_GRAD,
  RADIAL_GRAD,
  FILL,
  SHADOW_INSET,
  SHADOW_OUTSET,
};
enum unit_types { UNIT_FLEX = 1, UNIT_VH, UNIT_VW, UNIT_REL };

enum over_flow_types {
  SHOW = 1,
  HIDDEN,
};

struct flexon_unit_factor {
  float remFactor;
  float vhFactor;
  float vwFactor;
};

#endif
