#include "../flexon_modules/flexon.hpp"
#include "../flexon_modules/components/View.hpp"
#include "../flexon_modules/components/decorator.hpp"
#include "../flexon_modules/components/matrices.hpp"
#include "../flexon_modules/flexon.hpp"
#include <iostream>


window flexon::window = {
.flag = WINDOW_FULL_SCREEN,
.name =(char *)"hello world",
.width = 10,
.height = 100000
};

void flexon::appmain(){

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

};


