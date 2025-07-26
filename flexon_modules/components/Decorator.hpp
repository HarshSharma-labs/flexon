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
  modifier() {
    int i = 1;
    if (*(char *)&i) {
      little_endian_system = true;
    }
  };

  void mount(flexonView *_point) {
    mountp = _point;
    modifierStyle = &(*_point).style;
    modifierlay = &(*_point).layout;
    modifierStyle->borderColorCount = 0;
    modifierStyle->bgColorCount = 0;
    return;
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

    return *this;
    static_assert(std::conjunction_v<std::is_same<refType, variadicColor>...>);
    const std::size_t argsSize = sizeof...(args);
    std::size_t getArg = 0;
    int count = 0;

    if (type != nullptr) {
      int count = 0;
      while (count[type] != '\0') {
        if (count[type] == '%') {
          count += 1;
          switch (count[type]) {
          case 'c':
            // modifierStyle->backgroundColor = new colornc;
            // modifierStyle->bgColorCount += 1;

            break;
          case 'b':
            count += 1;
            switch (count[type]) {
            case 'g':
              std::cout << "backgroundColor here" << std::endl;
              break;
            case 't':
              std::cout << "borderTop here" << std::endl;
              break;
            case 'l':
              std::cout << "borderLeft here" << std::endl;
              break;
            case 'r':
              std::cout << "borderRight here" << std::endl;
              break;
            case 'b':
              std::cout << "borderbottom here" << std::endl;
              break;
            default:
              std::cout << "borderColor here" << std::endl;
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
    size_t spaceCount = sizeof...(variadicSpace);
    int floatCount = 0;

    if (type != nullptr) {
      int count = 0;
      uniparam *spaceContext = nullptr;
      while (count[type] != '\0') {
        if (count[type] == '%') {
          count += 1;
          switch (count[type]) {
          case 'm':

            break;
          case 'p':
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

#ifdef FLEXON_ENABLE_STATIC_LAYOUT
  modifier &setDimensions(float hight, float width) {
    (*modifierlay).height = heigth;
    (*modifierlay).width = width;
    return *this;
  };
  modifier &fillMaxSize();

#endif // FLEXON_ENABLE_STATIC_LAYOUT
#ifdef FLEXON_ENABLE_FLEX_LAYOUT
  modifier &setFlex(float flex) {
    (*modifierlay).flex = flex;
    return *this;
  };
#endif

#ifdef FLEXON_ENABLE_SEMIDYNAMIC_LAYOUT
  modifier &rem(float value);
  modifier &vh(float value);
  modifier &vw(float value);
  modifier &rel(float value);
#endif //

private:
  flexonView *mountp = {nullptr};
  decorator *modifierStyle;
  lay *modifierlay;

  bool little_endian_system{false};
};

extern modifier<uint32_t> Modifier;
#endif
