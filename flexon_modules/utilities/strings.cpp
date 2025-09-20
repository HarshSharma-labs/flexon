#include "./utility.hpp"

void utility::memcpy64(uint64_t *__src , uint64_t *__dst ,size_t size64){
 for(size_t i = 0; i < size64 ; i++){
     __dst[i] = __src[i];
  };
};

void utility::memset64(void* __src , uint64_t ch , size_t size64){
  uint64_t *__dst = (uint64_t*)__src;
  for(size_t i = 0; i < size64 ; i++){
     __dst[i] = ch;
  };
};

uint32_t utility::fnv_1hash(uint32_t value){
  uint32_t hash = 0x811c9dc5;
  char *data = (char *)&value;

  for(int i = 0 ; i < sizeof(uint32_t); i++){
    hash *= 0x01000193;
    hash ^= data[i]; 
   }

   return hash;
};

