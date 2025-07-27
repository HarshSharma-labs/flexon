#include "../componentbase/flexonGlobal.hpp"
#include "defs.hpp"
#include <cstdint>
#include <iostream>
#include <ostream>

struct flexon_unit_factor *ref_unit_factor = nullptr;

void unitMountFactor(struct flexon_unit_factor *mount_point) {
  ref_unit_factor = mount_point;
};
uint32_t hex(int code) {
  uint32_t shiftedColor = static_cast<uint32_t>(code);
  return shiftedColor;
}

float rem(float value) { return (value / 100) * ref_unit_factor->remFactor; };

float vh(float value) {
  // (*global_stitch)->layout.height_unit = UNIT_VH;
  return value * ref_unit_factor->vhFactor;
};
float vw(float value) {
  //(*global_stitch)->layout.width_unit = UNIT_VW;
  return value * ref_unit_factor->vwFactor;
};
float relh(float value) { return value; };
float relw(float value) { return value; };

void print_node_diagonistics(struct base_view *node) {
  std::cout << " Node diagonostics :" << std::endl;
  std::cout << " Node mem addr                            : " << std::hex
            << node << std::endl;
  std::cout << " Node style : " << std::endl;
  std::cout << "   Node {height}{width}                   : {"
            << node->layout.height << "} {" << node->layout.width << "} "
            << std::endl
            << "   Node padding{t,l,r,b}                  : {"
            << node->layout.padding.top << "," << node->layout.padding.left
            << "," << node->layout.padding.right << ","
            << node->layout.padding.bottom << "}"
            << "\n"
            << "   Node margin{t,l,r,b}                   : {"
            << node->layout.margin.top << "," << node->layout.margin.left << ","
            << node->layout.margin.right << "," << node->layout.margin.bottom
            << "}"
            << "\n"
            << "   Node border{t,l,r,b}                   : {"
            << node->layout.Border.top << "," << node->layout.Border.left << ","
            << node->layout.Border.right << "," << node->layout.Border.bottom
            << "}"
            << "\n"
            << "   Node flex                              : {"
            << node->layout.flex << "}"
            << "\nNode Decoration(colors) : " << std::endl
            << "   Node bgColor{rgba}                     : {"
            << static_cast<int>(node->style.color.bgColor.r) << ","
            << static_cast<int>(node->style.color.bgColor.g) << ","
            << static_cast<int>(node->style.color.bgColor.b) << ","
            << static_cast<int>(node->style.color.bgColor.a) << "}\n"
            << "   Node Color{rgba}                       : {"
            << static_cast<int>(node->style.color.color.r) << ","
            << static_cast<int>(node->style.color.color.g) << ","
            << static_cast<int>(node->style.color.color.b) << ","
            << static_cast<int>(node->style.color.color.a) << "}\n"
            << "   Node border{t|b|l|r}                   : {"
            << static_cast<int>(node->style.color.border.top.r) << ","
            << static_cast<int>(node->style.color.border.top.g) << ","
            << static_cast<int>(node->style.color.border.top.b) << ","
            << static_cast<int>(node->style.color.border.top.a) << "}{"
            << static_cast<int>(node->style.color.border.bottom.r) << ","
            << static_cast<int>(node->style.color.border.bottom.g) << ","
            << static_cast<int>(node->style.color.border.bottom.b) << ","
            << static_cast<int>(node->style.color.border.bottom.a) << "}{"
            << static_cast<int>(node->style.color.border.left.r) << ","
            << static_cast<int>(node->style.color.border.left.g) << ","
            << static_cast<int>(node->style.color.border.left.b) << ","
            << static_cast<int>(node->style.color.border.left.a) << "}{"
            << static_cast<int>(node->style.color.border.right.r) << ","
            << static_cast<int>(node->style.color.border.right.g) << ","
            << static_cast<int>(node->style.color.border.right.b) << ","
            << static_cast<int>(node->style.color.border.right.a) << "}\n";

  return;
}
