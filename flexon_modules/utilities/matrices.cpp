#include "../components/matrices.hpp"
#include "./utility.hpp"
#include <iostream>

mat3x2 <float> NDCmat = {0};

mat4x2<float> utility::ndc::NDCgen(mat4x3<float> &vertex){

    mat4x2<float> eval = {0.0f};

    float *ptrA[3] = {(float *)&NDCmat.x, (float *)&NDCmat.y, (float *)&NDCmat.z};
    float *ptrB[4] = {(float *)&vertex.x, (float *)&vertex.y, (float *)&vertex.z, (float *)&vertex.w};
    float *ptrE[4] = {(float *)&eval.x, (float *)&eval.y, (float *)&eval.z, (float *)&eval.w};

    for (int i = 0; i < 4; i++) { // Rows of vertex
        for (int j = 0; j < 2; j++) { // Columns of matrix
            for (int k = 0; k < 3; k++) { // Shared dimension
                ptrE[i][j] += ptrB[i][k] * ptrA[k][j]; 
            }
      } 
    }

  return eval;
}

void utility::ndc::setNDCmat(float width,float height){
float dw = 2/width;
float dh =  2/height;
NDCmat = {
    { dw , 0  },
    { 0  , -dh },
    { -1 , 1 }};
return;
};
