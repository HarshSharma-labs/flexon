#include "../flexon_modules/flexon.hpp"
#include "../flexon_modules/components/View.hpp"
#include "../flexon_modules/components/decorator.hpp"
#include "../flexon_modules/components/matrices.hpp"
#include <iostream>


void post_startup(fiber *wrap){
 modifier root;
  root.context(wrap).display('f').layoutDirection('c')
    .windowDimension(500.0f,500.0f)
    .padding({0.0f,20.0f,20.0f,10.0f})
    .borderWidth({10.0f,0.0f,20.0f,0.0f})
    .commitContext();
};

void app_main(){

View([](fiber *wrap){
     modifier style;
     style.context(wrap).display('f').layoutDirection('c').flex(1.0f).commitContext();
  },[](){
/*
View([](fiber *wrap){
     modifier style;
     style.context(wrap).flex(1.0f).layoutDirection('r').commitContext();
  },[](){
   });
*/

});

View([](fiber *wrap){
    modifier style;
    style.context(wrap).display('n').flex(1.0f).commitContext();
  },[](){
 });

}


