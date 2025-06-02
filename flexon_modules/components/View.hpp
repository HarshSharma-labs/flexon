#ifndef __FLEXON_VIEW_HPP
#define __FLEXON_VIEW_HPP
#include "../componentbase/component.hpp"
#include "../componentbase/flexonGlobal.hpp"
class View {
public:
  View(functionSignatureVoid __call) {
    knot();
    __call();
    globalStitch = &(**attachto).neighbour;
  };
  View(functionSignature __call) {
    knot();
    __call(*attachto);
    globalStitch = &(**attachto).neighbour;
  }

private:
  flexonView **attachto = globalStitch;
  void knot() {
    (*attachto) = new flexonView;
    globalStitch = &(**attachto).child;
  };
};
#endif
