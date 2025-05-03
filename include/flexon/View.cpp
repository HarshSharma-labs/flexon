#include "View.hpp"
#include "composable.hpp"
#include "view.hpp"
#include <cstdlib>
#include <functional>

flexon_view **tmp = {nullptr};

View::View(std::function<void(mine)> composable) {

  flexon_compose_tree(&tmp, composable);
  return;
}
