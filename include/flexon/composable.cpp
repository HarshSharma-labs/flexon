#include "composable.hpp"
#include "../../renderer/GUI/shaderwrapper.hpp"

flexonView *flexon_compose_tree(flexonView ***__tmpmine,
                                std::function<void(mine)> &__callcomposable) {

  SHADER::countView += 1;
  flexonView *tmpself = new flexonView;
  flexonView **pass = &(tmpself->children);
  *(*__tmpmine) = tmpself;
  *__tmpmine = pass;
  __callcomposable(tmpself);
  *__tmpmine = &((*tmpself).neighbour);
  return tmpself;
}

flexonView *flexon_compose_tree(flexonView ***__tmpmine,
                                std::function<void()> &__callcomposable) {

  SHADER::countView += 1;
  flexonView *tmpself = new flexonView;
  flexonView **pass = &(tmpself->children);
  *(*__tmpmine) = tmpself;
  *__tmpmine = pass;
  __callcomposable();
  *__tmpmine = &((*tmpself).neighbour);
  return tmpself;
}
