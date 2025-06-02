#ifndef __FLEXON__
#define __FLEXON__

#include "./componentbase/component.hpp"
#include "./componentbase/flexonGlobal.hpp"
#include <cstdio>
#include <iostream>

typedef struct baseView *capture;

class flexon {
public:
  flexon(functionSignatureVoid callchild) {
    flexonView **passView = &flexonTree.relativeTree.child;
    globalStitch = passView;
    callchild();
    for (flexonView *tmp = flexonTree.relativeTree.child; tmp;
         tmp = tmp->neighbour)
      std::cout << "incurred child" << tmp->layout.height << std::endl;
  };

private:
  flexonGuiTree flexonTree;
};

class flexonModifier {
public:
  flexonModifier(capture cap) { modifierView = cap; };
  void printHello() { std::cout << "HEllOWorld" << std::endl; }

private:
  flexonView *modifierView{nullptr};
};

#endif
