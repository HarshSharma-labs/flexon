#include "composable.hpp"

flexon_view *flexon_compose_tree(flexon_view ***__tmpmine,
                                 std::function<void(mine)> &__callcomposable) {

  flexon_view *tmpself = new flexon_view;
  flexon_view **pass = &(tmpself->children);
  *(*__tmpmine) = tmpself;
  *__tmpmine = pass;
  __callcomposable(tmpself);
  *__tmpmine = &((*tmpself).neighbour);
  return tmpself;
}
