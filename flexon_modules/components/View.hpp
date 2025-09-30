#ifndef __FLEXON_VIEW_HPP
#define __FLEXON_VIEW_HPP

#include "./base.hpp"



typedef struct pview{
 void(*style)(fiber *wrap) = nullptr;
 void(*child)() = nullptr;
 pview *next = nullptr;
}pview;

void View(void(*style)(fiber *wrap),void(*child)());
   

pview *gettop();
void flushViewTree();
#endif
