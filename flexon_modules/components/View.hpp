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
    global_parent_stitch = *attachto;
    global_stitch = &(**attachto).neighbour;
  };

private:
  flexon_view **attachto = global_stitch;
  void knot() {
    (*attachto) = new flexon_view;
    (*attachto)->parent = global_parent_stitch;
    global_parent_stitch = *attachto;
    Modifier.mount(*attachto);
    global_stitch = &(**attachto).child;
  };
};
#endif
