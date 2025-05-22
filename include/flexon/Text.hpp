#ifndef __FLEXON_TEXT_COMP__
#define __FLEXON_TEXT_COMP__
#include "../flexon.hpp"
#include "composable.hpp"
#include <cassert>
#include <functional>
class Text {
public:
  Text(std::function<void(mine)> composable) {
    flexonView *textNode = flexon_compose_tree(&tmp, composable);
    assert(textNode != nullptr);
    textNode->isViewText = true;
  }
};
#endif
