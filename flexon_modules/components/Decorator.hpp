#ifndef __FLEXON_DECORATOR_HPP__
#define __FLEXON_DECORATOR_HPP__

#include "../componentbase/component.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <type_traits>

template <typename refType> class modifier {
public:
  void mount(flexon_view *point) {
    mountp = point;
    modifierStyle = &(*point).style;
    modifierlay = &(*point).layout;

    memset(modifierlay, 0, sizeof(lay));
    memset(modifierStyle, 0, sizeof(decorator));

    (*modifierStyle).position = POSITION_RELATIVE;
    (*modifierStyle).color.color_type = default_color_fill_type;
    (*modifierStyle).color.bg_fill_type = default_color_fill_type;
    (*modifierStyle).color.border_fill_type = default_color_fill_type;

    (*modifierStyle).display = default_display_type;
    default_display_type == DISPLAY_FLEX
        ? default_layout_direction == LAYOUT_COLOUMN
              ? (*modifierlay).height_unit = UNIT_FLEX
              : (*modifierlay).width_unit = UNIT_FLEX
        : (*modifierlay).height_unit = UNIT_VH;

  }; // { mountp = _point; }

  /*
   * color function:
   * - format of arg: "key"
   *
   *  type of key :
   *     1) color            : %c
   *     2) border           : %b
   *     3) background       : %bg
   *     4) borderTop        : %bt
   *     5) borderBottom     : %bb
   *     6) borderLeft       : %bl
   *     7) borderRight      : %br
   *  To use gradient use format:
   *      (key):%(l|c|r)g:n
   *     :- Here n = number of color;
   *
   * parameter value can take:
   *    1) hex - 0x000000
   *    2) rgb - 125, 100, 255
   *    3) rgba - 255, 255, 255
   *    4) hexa - 0x000000ff
   *
   *  Usages:
   *   Color("%b",0x000000);
   *   Color("%lg %bg",0xff0012,0xff033ff);
   *
   */

  template <typename... variadicColor>

  modifier &Color(const char *type, variadicColor... args) {

    static_assert(std::conjunction_v<std::is_same<refType, variadicColor>...>);
    const size_t args_size = sizeof...(args);
    refType colors[args_size] = {args...};
    std::size_t arg_count = 0;
    int count = 0;

    if (type != nullptr) {
      int count = 0;
      while (count[type] != '\0') {
        if (count[type] == '%') {

          count += 1;

          switch (count[type]) {

          case 'c':

            fillhex(&arg_count[colors], &(*modifierStyle).color.color);
            arg_count += 1;

            break;
          case 'b':

            count += 1;
            switch (count[type]) {
            case 'g':
              fillhex(&arg_count[colors], &(*modifierStyle).color.bgColor);
              arg_count += 1;

              break;
            case 't':

              fillhex(&arg_count[colors], &(*modifierStyle).color.border.top);
              arg_count += 1;

              break;
            case 'l':

              fillhex(&arg_count[colors], &(*modifierStyle).color.border.left);
              arg_count += 1;

              break;
            case 'r':

              fillhex(&arg_count[colors], &(*modifierStyle).color.border.right);
              arg_count += 1;

              break;
            case 'b':

              fillhex(&arg_count[colors],
                      &(*modifierStyle).color.border.bottom);
              arg_count += 1;

              break;
            default:

              fillhex(&arg_count[colors], &(*modifierStyle).color.border.top);
              fillhex(&arg_count[colors],
                      &(*modifierStyle).color.border.bottom);
              fillhex(&arg_count[colors], &(*modifierStyle).color.border.left);
              fillhex(&arg_count[colors], &(*modifierStyle).color.border.right);
              arg_count += 1;
              count -= 1;
              break;
            }
            break;
          default:
            count -= 1;
            break;
          }
        }
        count += 1;
      }
    }
    return *this;
  };

  // modifier &setBorderWidth(const char *each,...);

  /*
   * space function:
   *  - format of arg:"key:value"
   *
   * type of key:
   *     1) padding         : %p
   *     2) paddingTop      : %pt
   *     3) paddingLeft     : %pl
   *     4) paddingRight    : %pr
   *     5) paddingBottom   : %pb
   *
   *     6) margin          : %m
   *     7) marginTop       : %mt
   *     8) marginLeft      : %ml
   *     9) marginRight     : %mr
   *    10) marginBottom    : %mb
   *
   *    11) gap             : %g
   */
  template <typename... variadicSpace>

  modifier &space(const char *type, variadicSpace... arg) {

    static_assert(std::conjunction_v<std::is_same<float, variadicSpace>...>,
                  "space function only support floating type use units utility "
                  "to pass arguements to modifier");
    const size_t arg_size = sizeof...(variadicSpace);
    float spaces_value[arg_size] = {arg...};
    size_t arg_count = 0;

    if (type != nullptr) {
      size_t count = 0;
      uni_param *spaceContext = nullptr;

      while (count[type] != '\0') {
        if (count[type] == '%') {
          count += 1;
          switch (count[type]) {
          case 'm':
            count += 1;
            std::cout << spaces_value[0] << std::endl;
            fill_uni_param(&(*modifierlay).margin, &count[type], &count,
                           &arg_count, &spaces_value[0]);
            break;
          case 'p':
            count += 1;
            fill_uni_param(&(*modifierlay).padding, &count[type], &count,
                           &arg_count, &spaces_value[0]);

            break;
          default:
            count -= 1;
            break;
          }
        }
        count += 1;
      }
    }
    return *this;
  }

  /*
   * positon function:
   *  - format of args: "key"
   *
   *  type of key:
   *    1) relative          : %r
   *    2) absolute          : %a
   */
  modifier &setPosition(const char *key) {
    if (key != nullptr) {
      int count = 0;
      while (count[key] != '\0') {
        if (count[key] == '%') {
          count += 1;
          switch (count[key]) {
          case 'r':
            // found relative
            (*modifierStyle).position = POSITION_RELATIVE;
            break;
          case 'a':
            // found absolute
            (*modifierStyle).position = POSITION_ABSOLUTE;
            break;
          default:
            count -= 1;
            break;
          }
        }
        count += 1;
      }
    }
    return *this;
  }

  /*
   *  justifyContent function:
   *   - format of args : "key"
   *
   *   type of key:
   *    1) flex-start       : %fs
   *    2) flex-end         : %fe
   *    3) center           : %c
   *    4) space-between    : %sb
   *    5) space-around     : %sa
   *    6) space-evenly     : %se
   *    7) start            : %s
   *    8) end              : %e
   */

  modifier &justifyContent(const char *key) {
    if (key != nullptr) {
      int count = 0;
      while (count[key] != '\0') {
        if (count[key] == '%') {
          count += 1;
          switch (count[key]) {
          case 'c':
            // found center
            (*modifierStyle).justifyItems = JUSTIFY_CENTER;
            break;
          case 'e':
            // found end
            (*modifierStyle).justifyItems = JUSTIFY_END;
            break;
          case 'f':
            count += 1;
            switch (count[key]) {
            case 's':
              // found flex-start
              (*modifierStyle).justifyItems = JUSTIFY_FLEX_START;

              break;
            case 'e':
              // found flex-end
              (*modifierStyle).justifyItems = JUSTIFY_FLEX_END;

              break;
            default:
              count -= 1;
              break;
            }
            break;
          case 's':
            count += 1;
            switch (count[key]) {
            case 'a':
              // found space-around
              (*modifierStyle).justifyItems = JUSTIFY_SPACE_AROUND;

              break;
            case 'b':
              // found space-between
              (*modifierStyle).justifyItems = JUSTIFY_SPACE_BETWEEN;

              break;
            case 'e':
              // found space-evenly
              (*modifierStyle).justifyItems = JUSTIFY_SPACE_EVENLY;

              break;
            default:
              // found start
              (*modifierStyle).justifyItems = JUSTIFY_START;

              count -= 1;
              break;
            }
            break;
          default:
            count -= 1;
            break;
          }
        }
        count += 1;
      }
    }
    return *this;
  }

  /*
   * alignitems function:
   *  - format of args: "key"
   *
   *  type of key:
   *    1) center           : %c
   *    2) baseline         : %b
   *    3) stretch          : %s
   *    4) flex-start       : %fs
   *    5) flex-end         : %fe
   */

  modifier &alignItems(const char *key) {
    if (key != nullptr) {
      int count = 0;
      while (count[key] != '\0') {
        if (count[key] == '%') {
          count += 1;
          switch (count[key]) {
          case 'c':
            // found center
            (*modifierStyle).alignitems = ALIGN_CENTER;

            break;
          case 'b':
            // found baseline
            (*modifierStyle).alignitems = ALIGN_BASELINE;

            break;
          case 's':
            // found stretch
            (*modifierStyle).alignitems = ALIGN_STRETCH;

            break;
          case 'f':
            count += 1;
            switch (count[key]) {
            case 's':
              // found flex-start
              (*modifierStyle).alignitems = ALIGN_FLEX_START;

              break;
            case 'e':
              // found flex-end
              (*modifierStyle).alignitems = ALIGN_FLEX_END;

              break;
            default:
              count -= 1;
              break;
            }
            break;
          default:
            count -= 1;
            break;
          }
        }
        count += 1;
      }
    }
    return *this;
  }

  /*
   * onEvent function:
   *  - format of args : "key"
   *
   *  type of key:
   *    1) onCLick          : %oc
   *    2) onHover          : %oh
   *    3) onScroll         : %os
   *    4) onLayoutChange   : %olc
   *
   */

  modifier &onEvent(const char *type, ...);

  modifier &Dimensions(float height, float width) {
    (*modifierlay).height = height;
    (*modifierlay).width = width;
    return *this;
  }
  modifier &Width(float width) {
    (*modifierlay).width = width;
    return *this;
  };
  modifier &Height(float height) {
    (*modifierlay).height = height;
    return *this;
  }

  modifier &fillMaxSize() {
    (*modifierlay).height_unit = UNIT_FILL_MAX;
    return *this;
  };

  modifier &setFlex(float flex) {
    (*modifierlay).flex = flex;
    (*modifierStyle).layout_direction == LAYOUT_COLOUMN
        ? (*modifierlay).height_unit = UNIT_FLEX
        : (*modifierlay).width_unit = UNIT_FLEX;
    return *this;
  };

private:
  flexon_view *mountp = {nullptr};
  decorator *modifierStyle;
  lay *modifierlay;

  enum display_types default_display_type = DISPLAY_FLEX;
  enum layout_directions default_layout_direction = LAYOUT_COLOUMN;
  enum color_fill_type default_color_fill_type = COLOR_FILL;
  enum over_flow_types default_over_flow_type = OVER_SHOW;

  void fillhex(uint32_t *arg, colornc *color_ref) {
    color_ref->r = (*arg >> 24) & 0xff;
    color_ref->g = (*arg >> 16) & 0xff;
    color_ref->b = (*arg >> 8) & 0xff;
    color_ref->a = *arg & 0xff;
  };
  void fill_uni_param(uni_param *which, const char *next, size_t *count_ref,
                      size_t *arg_count_ref, float *spaces_value) {

    size_t sub_arg_count = *arg_count_ref;

    switch (*next) {
    case 't':
      which->top = sub_arg_count[spaces_value];
      *arg_count_ref += 1;
      break;
    case 'b':
      which->bottom = sub_arg_count[spaces_value];
      *arg_count_ref += 1;

      break;
    case 'l':
      which->left = sub_arg_count[spaces_value];
      *arg_count_ref += 1;

      break;
    case 'r':
      which->right = sub_arg_count[spaces_value];
      *arg_count_ref += 1;

      break;
    default:
      which->top = sub_arg_count[spaces_value];
      which->bottom = sub_arg_count[spaces_value];
      which->right = sub_arg_count[spaces_value];
      which->left = sub_arg_count[spaces_value];
      *arg_count_ref += 1;
      *count_ref -= 1;
      break;
    }
  };
};

extern modifier<uint32_t> Modifier;
#endif
