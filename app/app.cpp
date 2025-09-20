#include "../flexon_modules/flexon.hpp"
#include "../flexon_modules/components/View.hpp"
#include "../flexon_modules/components/decorator.hpp"
#include "../flexon_modules/components/matrices.hpp"
#include <iostream>


void post_startup(fiber *wrap){
 modifier root;
  root.context(wrap).display('n').dimension(500.0f,500.0f).commitContext();
};

void app_main(){

View([](fiber *wrap){
      std::cout<<"hello style"<<std::endl;
  },[](){
    std::cout<<"child1"<<std::endl;
  View([](fiber *wrap){
       std::cout<<"child ka child ka style"<<std::endl;
     },[](){
       std::cout<<"child ka child"<<std::endl;
     });
  });

View([](fiber *wrap){
   std::cout<<"style child"<<std::endl;
  },[](){
  std::cout<<"child2"<<std::endl;
 });

}


