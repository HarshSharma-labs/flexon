#include "../flexon_modules/flexon.hpp"
#include "../flexon_modules/components/View.hpp"
#include "../flexon_modules/components/decorator.hpp"
#include "../flexon_modules/components/matrices.hpp"
#include "../flexon_modules/flexon.hpp"
#include <iostream>


void flexon::post_startup(fiber *wrap , vec2<float> &windowDimension){
 modifier root;
  root.context(wrap).display('f').layoutDirection('r')
    .windowDimension(10.0f,10.0f)
    .padding({20.0f,20.0f,20.0f,20.0f})
    .borderWidth({20.0f,10.0f,40.0f,30.0f})
    .commitContext();
};

void flexon::app_main(){

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
    modifier::onPress([](void *args){
      std::cout<<"child"<<std::endl;
     });
    modifier::commitEvent();

 });

});

View([](fiber *wrap){
    modifier style;
    style.context(wrap).display('f').flex(1.0f).commitContext();
  },[](){
 
 });

}


