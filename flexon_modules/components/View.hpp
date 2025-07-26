#ifndef __FLEXON_VIEW_HPP
#define __FLEXON_VIEW_HPP
#include "../componentbase/component.hpp"
#include "../componentbase/flexonGlobal.hpp"
#include "./Decorator.hpp"

class View {
public:
  template <typename funcSig> View(funcSig __call) {
    knot();
    __call();
    globalStitch = &(**attachto).neighbour;
  };

private:
  flexonView **attachto = globalStitch;
  void knot() {
    (*attachto) = new flexonView;
    Modifier.mount(*attachto);
    globalStitch = &(**attachto).child;
  };
};
#endif
