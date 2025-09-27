#ifndef __FLEXON_DECORATOR_HPP__
#define __FLEXON_DECORATOR_HPP__

#include "./View.hpp"
#include "./base.hpp"
#include "../utilities/utility.hpp"
#include "../subsystem/statemanager/statemanager.hpp"
#include <unordered_map>
#include <iostream>
#include <cstddef>
#include <bitset>
#include <cstdint>
#include <cstring>


/* uniparam :
 * fields to vec4 translation:
 *  
 *  top = x in vec4.
 *  bottom = y in vec4.
 *  right = z in vec4.
 *  left = w in vec4.
 */

/*
 * [CRITICAL SECTION DON'T REORDER FIELDS]
 */
typedef struct uniparam{
 float top;     
 float bottom;  
 float right;    
 float left; 
}uniparam;

typedef struct runiparam{
 float topLeft;
 float topRight;
 float bottomLeft;
 float bottomRight;

}runiparam;
/*
 * [CRITICAL SECTION END]
 */

class modifier {
public:

  
    modifier &context(fiber *ctx){

     mine = ctx;
     paint.display = DISPLAY_NONE;
     paint.visible = VIEW_HIDE;
     memset(&paint,'\0',sizeof(base_paint));
     activeFiberId = ctx->fiberid;
     return *this;
   };

  
   modifier &commitContext(){
     base_paint *tmp = new base_paint;
          
     ppaint[mine->fiberid] = tmp;
     mine->paint = tmp;
     utility::strings::memcpy64(&paint,tmp ,paintsize64);

        return *this;
   };

   //only operate on text
   modifier &color(uint32_t value){  return *this;};

   modifier &backgroundColor(uint32_t value){
    if(paint.bgColor != nullptr) return *this;
     paint.bgColor = (uint32_t *)new struct normal_fill;
    *paint.bgColor = COLOR_FILL;
    *(paint.bgColor+1) = value;
    return *this; 
  };

/*
  //only linear gradient;
   modifier &enablegradient(){
      if(mine.paint.bgColor != nullptr) return;
   return *this;

   modifier &gradientstart(float x, float y);
   modifier &gradientstop(float x,float y);
   modifier &gradient(uint32_t color,float stop);
*/

   modifier &margin(float value){
    paint.geometry.margin.x = value;
    paint.flagpadmar |= UNITYPE_MARGIN_ALL; 
    return *this;
   };

   modifier &margin(uniparam margins){
   memcpy(&paint.geometry.margin,&margins ,sizeof(uniparam));
   utility::decorator::setbits(&paint.flagpadmar,(float*)&margins , UNITYPE_STRIDE_MARGIN);

   return *this;
   };

   modifier &padding(float value){
   paint.geometry.padding.x = value;
   paint.flagpadmar |= UNITYPE_PADDING_ALL;

   return *this;
   };

   modifier &padding(uniparam paddings){
   memcpy(&paint.geometry.padding.x,&paddings ,sizeof(uniparam));
   utility::decorator::setbits(&paint.flagpadmar,(float*)&paddings , UNITYPE_STRIDE_PADDING);
   return *this;
   };


   modifier &borderWidth(float value){
   paint.geometry.borderWidth.x = value;
   paint.flagborrad |= UNITYPE_BORDERWIDTH_ALL;
 
   return *this;
   };

   modifier &borderWidth(uniparam widths){
   memcpy(&paint.geometry.borderWidth,&widths,sizeof(uniparam));
   utility::decorator::setbits(&paint.flagborrad,(float*)&widths , UNITYPE_STRIDE_BORDERWIDTH);
 
    return *this;
   };

   modifier &borderRadius(float value){
   paint.geometry.cornerRadius.x = value;
   paint.flagborrad |= UNITYPE_BORDERRADIUS_ALL;
   return *this;
   };

   modifier &borderRadius(runiparam radius){
   memcpy(&paint.geometry.cornerRadius,&radius ,sizeof(runiparam));
   utility::decorator::setbits(&paint.flagborrad,(float*)&radius, UNITYPE_STRIDE_BORDERRADIUS);
 
    return *this;
  };

   modifier &dimension(float width,float height){
    switch(paint.display){
     case DISPLAY_FLEX:
     case DISPLAY_GRID:
     case DISPLAY_NONE:
        return *this;
     break;
    };
     paint.visible = VIEW_HIDE;
     paint.display = DISPLAY_CUSTOM;
     paint.geometry.dimension.z = width;
     paint.geometry.dimension.w = height;
     return *this;

   };
    modifier &windowDimension(float width , float height){
    if(mine->fiberid == 1){
      paint.geometry.dimension.z = width;
      paint.geometry.dimension.w = height;
       };
    return *this;
    };

    modifier &flex(float value){
    if(mine->parent != nullptr){
      switch(mine->parent->paint->display){
        case DISPLAY_NONE:
        case DISPLAY_GRID:
        case DISPLAY_CUSTOM:
         return *this;
        break;
      };
     };
     paint.flex = value;
     return *this;
    }

    /*
   *   display function:
   *   - format of args : "key"
   *
   *   type of key:
   *    1) none             : n
   *    2) flex             : f
   *    3) custom           : c
   *    4) grid             : g
   */

    modifier &display(char key){
     switch(key){
      case 'n':
       paint.display = DISPLAY_NONE;
      break;
      case 'f':
        paint.display = DISPLAY_FLEX;
      break;
      case 'c':
       paint.display = DISPLAY_CUSTOM;
      break;
      case 'g':
       paint.display = DISPLAY_GRID;
      break;
     };
      paint.layout_direction = LAYOUT_DIRECTION_COL;

     return *this;
    };
    
    modifier &layoutDirection(char key){
     switch(key){
      case 'c':
       paint.layout_direction = LAYOUT_DIRECTION_COL;
      break;
      case 'r':
       paint.layout_direction = LAYOUT_DIRECTION_ROW;
      break;
     };
     return *this;
    }
    /*
   *  justifyContent function:
   *   - format of args : "key"
   *
   *   type of key:
   *    1) flex-start       : fs
   *    2) flex-end         : fe
   *    3) center           : c
   *    4) space-between    : sb
   *    5) space-around     : sa
   *    6) space-evenly     : se
   *    7) start            : s
   *    8) end              : e
   */
   modifier &justifyContent(char* key){
    int keylen = strlen(key);
    if(keylen > 2) return *this;

    char seq[3];
    strcpy(seq,key);

    switch(seq[0]){
     case 'c':
       paint.justifyItems = JUSTIFY_CENTER;
       return *this;
      case 's':
         switch(seq[1]){
          case 'b':
           paint.justifyItems = JUSTIFY_SPACE_BETWEEN;
          return *this;
          case 'a':
          paint.justifyItems = JUSTIFY_SPACE_AROUND;
          return *this;
          case 'e':
          paint.justifyItems = JUSTIFY_SPACE_EVENLY;
          return *this;
          }
      paint.justifyItems = JUSTIFY_START;
      return *this;
      case'e':
      paint.justifyItems = JUSTIFY_FLEX_END;
      return *this;
      case 'f':
        switch(seq[1]){
          case 's':
           paint.justifyItems = JUSTIFY_FLEX_START;
          return *this;
          case 'e':
           paint.justifyItems = JUSTIFY_FLEX_END;
          return *this;

        }
    } 
     paint.justifyItems = JUSTIFY_START;
     return *this;
  };
  
  /*
   * alignitems function:
   *  - format of args: "key"
   *
   *  type of key:
   *    1) center           : c
   *    2) baseline         : b
   *    3) stretch          : s
   *    4) flex-start       : fs
   *    5) flex-end         : fe
   */
   modifier &alignItems(char* key);

  /* here parameter are as follow 
   * from = which paint attribute to copy
   * fiberid = id of the new fiber node that receives the clone
   */
   
  constexpr static  size_t paintsize64 = sizeof(base_paint)/ sizeof(uint64_t);
  constexpr static  size_t fibersize64 = sizeof(fiber)/ sizeof(uint64_t);
  constexpr static  size_t geosize64 = sizeof(base_geo) / sizeof(uint64_t);
  constexpr static  size_t vec4floatsize64 = sizeof(vec4<float>) / sizeof(uint64_t);

  static base_paint *deepClone(base_paint *from , uint32_t tofiberid){
    if(from == nullptr)
       return nullptr;

    base_paint *tmp = new base_paint;
    ppaint[tofiberid] = tmp;
    utility::strings::memcpy64(from , tmp ,paintsize64);
    return tmp; 
  };

  static void onPress(void(*callback)(void *args)){
    tmpcallbackholder.onpress = callback;
  };
  static void onLongPress(void(*callback)(void *args)){
    tmpcallbackholder.onLongPress = callback;
  };
  static void onHover(void(*callback)(void *args)){
    tmpcallbackholder.onHover = callback;
  };

  static void setActiveFiberId(fiber *again){
    mine = again;
  };

  static void commitEvent(){
    if(mine->paint != nullptr){
    statemanager::registerEventCallback(mine, tmpcallbackholder);
    }
  };

private:

  static inline fiber *mine = nullptr;
  base_paint paint;
  //base_geo paint.geometry;

  static inline callbackfunction tmpcallbackholder;

  /*
  * ppaint owns all the base_paint created within the flexon.
  * it's primary function if to have fast search of view data.
  * and allows synchronisation of latest paint among the view.
  */
  static inline uint32_t activeFiberId = 0;
  static inline std::unordered_map<uint32_t,base_paint*> ppaint;
 // static inline std::vector<normal_fill*> fill;
 // static inline std::vector<gradient_color*> gradient_fill;
 
 

 // provided by stylesheet; 
 // static map<rectView*,base *> stylesheet;
 //  static vector<sweep_gradient*> sweep_fill;
};

#endif
