#include "./View.hpp"
#include "../flexon.hpp"
#include <cstring>
#include <iostream>

static pview *top = nullptr;
static pview *evaluate = nullptr;



void View(void(*style)(fiber *wrap),void(*child)()){
 if(!style)
    return;

 if(top == nullptr){
     top = new pview;
     evaluate = top;
  
  }else{
  evaluate->next = new pview;
  evaluate = evaluate->next;
  };
   evaluate->style = style;
   evaluate->child = child;
};

pview *gettop(){
return top;
};

void flushViewTree(){
 

pview *tmp = top;

while(tmp != nullptr){  
   delete tmp;
   tmp = tmp->next;
};
  top = nullptr;
  evaluate = nullptr;
 
};
