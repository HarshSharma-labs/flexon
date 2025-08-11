#include "defs.hpp"
#include <cstdint>

struct flexon_unit_factor *ref_unit_factor = nullptr;

void unitMountFactor(struct flexon_unit_factor *mount_point) {
  ref_unit_factor = mount_point;
};
uint32_t hex(int code) {
  uint32_t shiftedColor = static_cast<uint32_t>(code);
  return shiftedColor;
}

float rem(float value) { return (value / 100) * ref_unit_factor->remFactor; };

float vh(float value) { return value * ref_unit_factor->vhFactor; };

float vw(float value) { return value * ref_unit_factor->vwFactor; };

float relh(float value) { return value; };
float relw(float value) { return value; };
