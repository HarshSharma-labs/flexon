#include "../components/matrices.hpp"

mat4x2<float> NDCgen(mat4x3<float> &vertex,mat3x2<float> &matrix){

    mat4x2<float> eval = {0.0f};

    float *ptrA[3] = {(float *)&matrix.x, (float *)&matrix.y, (float *)&matrix.z};
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
