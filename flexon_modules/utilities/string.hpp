#ifndef __FLEXON_STRING_UTILITIES__
#define __FLEXON_STRING_UTILITIES__

namespace flexon{
void memcpy64(uint64_t *__src , uint64_t *__dst ,size_t size64){
 for(size_t i = 0; i < size64 ; i++){
     __dst[i] = __src[i];
  };
};

void memset64(void* __src , uint64_t ch , size_t size64){
  uint64_t *__dst = (uint64_t*)__src;
  for(size_t i = 0; i < size64 ; i++){
     __dst[i] = ch;
  };
};
}

#endif
