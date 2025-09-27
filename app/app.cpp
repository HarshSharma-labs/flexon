#include "../flexon_modules/flexon.hpp"
#include "../flexon_modules/components/View.hpp"
#include "../flexon_modules/components/decorator.hpp"
#include "../flexon_modules/components/matrices.hpp"
#include <iostream>


void post_startup(fiber *wrap){
 modifier root;
  root.context(wrap).display('f').layoutDirection('r')
    .windowDimension(500.0f,500.0f)
    .padding({20.0f,20.0f,20.0f,20.0f})
    .borderWidth({20.0f,10.0f,40.0f,30.0f})
    .commitContext();
};

void app_main(){

View([](fiber *wrap){
     modifier style;
     style.context(wrap)
          .display('f')
          .layoutDirection('r')
          .padding({20.0f,20.0f,20.0f,20.0f})
          .borderWidth({20.0f,10.0f,40.0f,30.0f})
          .flex(1.0f).commitContext();
  },[](){
     modifier::onPress([](void *args){
      std::cout<<"hello world"<<std::endl;
     });
     modifier::commitEvent();
  
View([](fiber *wrap){
    modifier style;
    style.context(wrap).display('f').flex(1.0f).commitContext();
  },[](){
 
 });

});

View([](fiber *wrap){
    modifier style;
    style.context(wrap).display('n').flex(0.0f).commitContext();
  },[](){
 
 });

}


