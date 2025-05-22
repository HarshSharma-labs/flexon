#include "composable.hpp"

flexonView *flexon_compose_tree(flexonView ***__tmpmine,
                                std::function<void(mine)> &__callcomposable) {

  flexonView *tmpself = new flexonView;
  flexonView **pass = &(tmpself->children);
  *(*__tmpmine) = tmpself;
  *__tmpmine = pass;
  __callcomposable(tmpself);
  *__tmpmine = &((*tmpself).neighbour);
  return tmpself;
}
