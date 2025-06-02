#ifndef __VIEW_COMPOSABLE__
#define __VIEW_COMPOSABLE__

#include "../flexon.hpp"
#include <functional>
class View {
public:
  View(std::function<void(mine)>);
  View(std::function<void()>);
};

#endif
