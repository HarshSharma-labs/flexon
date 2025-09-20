#include "./layout.hpp"
#include <iostream>

/*
void calculatelayout(fiber *parent){
struct{

}pGeo;




};
*/


void deepCloneFiber(fiber *which){

};
//used to get a new fiber tree if there is a view added
fiber* getfiberfromview(pview * which){
   fiber *tmp = new fiber;
   tmp->child = which->child;
   if(which->style)
     which->style(tmp);

   return tmp;
};

void buildfibertree(void(*node)(), fiber *parent){

   node();
   pview *view = gettop();
   if(view == nullptr)
     return;

   fiber *topFiber = getfiberfromview(view);
   fiber *nextFiber = topFiber;
   fiber *tmp = nullptr;

  // function to create fiber from tree
   for(auto *itr = view->next ; itr != nullptr ; itr = itr->next){
     tmp = getfiberfromview(itr);   
     nextFiber->next = tmp;
     nextFiber = tmp;
   };
  
   flushViewTree();
//   calculatelayout();

  //function to evaluate the entire child and create the fiber
  for(auto *itr = topFiber; itr != nullptr; itr = itr->next){ 
    if(itr->child)
      buildfibertree(itr->child,itr); 
  };
};

/* here parameters are as follow:
 * void(*node)() = app_main() function in app.cpp file
 * width = width of the screen
 * height = height of the screen
*/


void initial_commit(void (*node)(),void(*main)(fiber *wrap)){
    //std::cout<<std::hex<<fnv_1hash(0xffcaffac)<<std::endl;

    return;

    fiber *fibertop = new fiber;
    main(fibertop);

    if(fibertop->paint == nullptr){
     std::cout<<"post startup failed exiting.\nprobable cause not" 
        " commiting a root layout in post_startup function"<<std::endl;
      return;
    };

   
   buildfibertree(node,fibertop); 
};



