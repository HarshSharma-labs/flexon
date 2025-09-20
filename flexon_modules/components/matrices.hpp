#ifndef __FLEXON_MATRICES__
#define __FLEXON_MATRICES__

template<typename T>
struct vec2{
 T x;
 T y;
};

template<typename T>
struct vec3{
 T x;
 T y;
 T z;
};

template<typename T>
struct vec4{
 T x;
 T y;
 T z;
 T w;
};

template<typename T>
struct mat3x2{
 vec2 <T> x;
 vec2 <T> y;
 vec2 <T> z;

};

template<typename T>
struct mat4x3{
 vec3 <T> x;
 vec3 <T> y;
 vec3 <T> z;
 vec3 <T> w;

};

template<typename T>
struct mat4x4{
 vec2 <T> x;
 vec2 <T> y;
 vec2 <T> z;
 vec2 <T> w;
};

template<typename T>
struct mat4x2{
 vec2 <T> x;
 vec2 <T> y;
 vec2 <T> z;
 vec2 <T> w;
};

mat4x2<float> NDCgen(mat4x3<float> &vertex,mat3x2<float> &matrix);

#endif
