#ifndef __FLEXON_COMPONENT_BASE__
#define __FLEXON_COMPONENT_BASE__



enum fill_types {
 NORMAL_FILL = 1,
 LINEAR_GRADIENT,
 SWEEP_GRADIENT,
 RADIAL_GRADIENT,
};

struct normal_fill{

};
struct linear_gradient{

};

struct sweep_gradient{

};

struct radial_gradient{

};

template<typename C>
typedef struct vec2{
  C x;
  C y;
}vec2;

template<typename T>
typedef struct vec4{
   T x;
   T y;
   T w;
   T h;
}vec4;

typedef struct callback{
 void(*pressed)(void *args);
 void(*longpressed)(void *args);
 void(*scrolled)(float x, float y);
 void(*onhover)(void *args);
}callback;


typedef struct base_quad{
  vec4 <float> s;
  vec2 <float> a;
  vec2 <float> b;
  vec2 <float> c;
  vec2 <float> d;
}base_quad;

typedef struct base_paint{
  void *bdColor;
  void *borderColor;
  
};

typedef struct base{
   
   struct base *parent;
   struct base *next;
   struct base *parent;
   callback  callback;

}base;

#endif
