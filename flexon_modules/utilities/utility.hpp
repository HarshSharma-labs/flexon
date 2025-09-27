#ifndef __FLEXON_STRING_UTILITIES__
#define __FLEXON_STRING_UTILITIES__

#include "../components/matrices.hpp"
#include <cstddef>
#include <cstdint>

namespace utility{
namespace strings{
 void memcpy64(void *__vsrc , void *__vdst ,size_t size64);
 void memset64(void* __src , uint64_t ch , size_t size64);
 uint32_t fnv_1hash(uint32_t &value);
}
namespace decorator{
 bool checkZero(uint8_t __vsrc , uint32_t stride);
 void setbits(uint8_t *__vsrc,float *param, uint32_t stride);
 bool emptyEntity(void *which , size_t size);

};
namespace ndc{

mat4x2<float> NDCgen(mat4x3<float> &vertex);
void setNDCmat(float width,float height);

};
};

#endif
