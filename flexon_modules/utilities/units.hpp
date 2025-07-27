#ifndef __FLEXON_UNITS_UTILITY__
#define __FLEXON_UNITS_UTILITY__
#include "../componentbase/component.hpp"
#include "defs.hpp"
#include <cstdint>

uint32_t hex(int color);
void unitMountFactor(struct flexon_unit_factor *);
float rem(float);
float vh(float);
float vw(float);
float rel(float);
void print_node_diagonistics(struct base_view *);
#endif
